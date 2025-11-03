/*
Name: Zhenjiang Tian
BlazerId: ztian
Project #: HW3 CS 532

To compile:
  make

To run:
  ./hw3 /path/to/directory
  ./hw3 -u username /path/to/directory
*/

#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <fcntl.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static int has_txt_suffix(const char *name) {
    size_t n = strlen(name);
    if (n < 4) return 0;
    return (strcmp(name + (n - 4), ".txt") == 0);
}

static long long count_words_whitespace_sep(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error: cannot open file '%s': %s\n", path, strerror(errno));
        return -1;
    }

    const size_t BUFSZ = 1 << 15;
    char *buf = (char*)malloc(BUFSZ);
    if (!buf) {
        fprintf(stderr, "Error: out of memory\n");
        close(fd);
        return -1;
    }

    long long words = 0;
    int in_word = 0;

    for (;;) {
        ssize_t r = read(fd, buf, BUFSZ);
        if (r < 0) {
            if (errno == EINTR) continue;
            fprintf(stderr, "Error: read '%s' failed: %s\n", path, strerror(errno));
            free(buf);
            close(fd);
            return -1;
        }
        if (r == 0) break;

        for (ssize_t i = 0; i < r; ++i) {
            if (isspace((unsigned char)buf[i])) {
                if (in_word) in_word = 0;
            } else {
                if (!in_word) {
                    ++words;
                    in_word = 1;
                }
            }
        }
    }

    free(buf);
    close(fd);
    return words;
}

static void print_usage_and_exit(const char *prog) {
    fprintf(stderr, "Usage: %s [-u username] <directory_name>\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    const char *owner_filter = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "u:")) != -1) {
        switch (opt) {
            case 'u':
                owner_filter = optarg;
                break;
            default:
                print_usage_and_exit(argv[0]);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Error: missing directory argument.\n");
        print_usage_and_exit(argv[0]);
    }

    const char *dirpath = argv[optind];

    struct stat st_dir;
    if (stat(dirpath, &st_dir) != 0) {
        fprintf(stderr, "Error: Directory not found: %s (%s)\n", dirpath, strerror(errno));
        return EXIT_FAILURE;
    }
    if (!S_ISDIR(st_dir.st_mode)) {
        fprintf(stderr, "Error: Not a directory: %s\n", dirpath);
        return EXIT_FAILURE;
    }

    DIR *dp = opendir(dirpath);
    if (!dp) {
        fprintf(stderr, "Error: cannot open directory '%s': %s\n", dirpath, strerror(errno));
        return EXIT_FAILURE;
    }

    struct dirent *de;
    int children = 0;

    while ((de = readdir(dp)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;

        char fullpath[PATH_MAX];
        int nw = snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, de->d_name);
        if (nw < 0 || (size_t)nw >= sizeof(fullpath)) {
            fprintf(stderr, "Warning: path too long, skipping: %s/%s\n", dirpath, de->d_name);
            continue;
        }

        struct stat st;
        if (lstat(fullpath, &st) != 0) {
            fprintf(stderr, "Warning: lstat failed on '%s': %s\n", fullpath, strerror(errno));
            continue;
        }

        if (!S_ISREG(st.st_mode)) continue;

        if (owner_filter) {
            struct passwd *pw = getpwuid(st.st_uid);
            const char *uname = pw ? pw->pw_name : NULL;
            if (!uname || strcmp(uname, owner_filter) != 0) {
                continue;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Error: fork failed for '%s': %s\n", fullpath, strerror(errno));
            for (int i = 0; i < children; ++i) {
                (void)wait(NULL);
            }
            return EXIT_FAILURE;
        } else if (pid == 0) {
            const char *fname = de->d_name;
            long long words = -1;
            int is_txt = has_txt_suffix(fname);

            if (is_txt) {
                words = count_words_whitespace_sep(fullpath);
            }

            if (owner_filter) {
                struct passwd *pw = getpwuid(st.st_uid);
                const char *uname = pw ? pw->pw_name : "unknown";
                if (is_txt) {
                    if (words >= 0) {
                        printf("File: %s | Size: %lld bytes | Words: %lld | Owner: %s\n",
                               fname, (long long)st.st_size, words, uname);
                    } else {
                        printf("File: %s | Size: %lld bytes | Words: N/A | Owner: %s\n",
                               fname, (long long)st.st_size, uname);
                    }
                } else {
                    printf("File: %s | Size: %lld bytes | Owner: %s\n",
                           fname, (long long)st.st_size, uname);
                }
            } else {
                if (is_txt) {
                    if (words >= 0) {
                        printf("File: %s | Size: %lld bytes | Words: %lld\n",
                               fname, (long long)st.st_size, words);
                    } else {
                        printf("File: %s | Size: %lld bytes | Words: N/A\n",
                               fname, (long long)st.st_size);
                    }
                } else {
                    printf("File: %s | Size: %lld bytes\n",
                           fname, (long long)st.st_size);
                }
            }

            fflush(stdout);
            _exit(EXIT_SUCCESS);
        } else {
            ++children;
        }
    }

    closedir(dp);

    int status;
    while (children-- > 0) {
        if (wait(&status) < 0) {
            if (errno == EINTR) { ++children; continue; }
            fprintf(stderr, "Warning: wait failed: %s\n", strerror(errno));
            break;
        }
        (void)status;
    }

    return EXIT_SUCCESS;
}
