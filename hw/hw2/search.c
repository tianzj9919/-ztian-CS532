/*
Name: Zhenjiang Tian
BlazerId: ztian
Project #: CS 532 HW2 search
To compile: make
To run: ./search [options] [start_dir]
*/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <getopt.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

typedef struct {
  int show_S;            // -S 
  off_t size_le;         // -s <bytes>
  const char *substr;    // -f <pattern>
  int max_depth;         // -f <depth> 
  char type_only;        // -t f|d
  const char *start_dir; 
} config_t;

typedef int  (*filter_fn)(
    const char *name,
    const struct stat *st,
    int is_dir,
    int depth,
    const config_t *cfg
);
typedef void (*print_fn )(
    const char *name,
    const struct stat *st,
    int is_dir,
    int depth,
    const config_t *cfg
);

static void print_indent(int depth) {
    for (int i = 0; i < depth; ++i) {
        putchar('\t');
    }
}

static void perm_str(mode_t m, char out[10]) {
    out[0]=(m&S_IRUSR)?'r':'-';
    out[1]=(m&S_IWUSR)?'w':'-';
    out[2]=(m&S_IXUSR)?'x':'-';
    out[3]=(m&S_IRGRP)?'r':'-';
    out[4]=(m&S_IWGRP)?'w':'-';
    out[5]=(m&S_IXGRP)?'x':'-';
    out[6]=(m&S_IROTH)?'r':'-';
    out[7]=(m&S_IWOTH)?'w':'-';
    out[8]=(m&S_IXOTH)?'x':'-';
    out[9] = '\0';
}

static void time_str(time_t t, char *buf, size_t n) {
    struct tm lt;
    localtime_r(&t, &lt);
    strftime(buf, n, "%Y-%m-%d %H:%M:%S", &lt);
}

static int pass_all(const char *name, const struct stat *st, int is_dir, int depth, const config_t *cfg) {
  if (cfg->type_only == 'f' && !S_ISREG(st->st_mode)) {
    return 0;
  }
  if (cfg->type_only == 'd' && !is_dir) {
    return 0;
  }

  if (cfg->size_le >= 0) {
    off_t sz = is_dir ? (off_t)0 : st->st_size;
    if (sz > cfg->size_le) {
        return 0;
    }
  }

  if (cfg->substr) {
    if (!strstr(name, cfg->substr)) {
        return 0;
    }
    if (depth > cfg->max_depth) {
        return 0;
    }
  }
  return 1;
}

static void print_basic(const char *name, const struct stat *st, int is_dir, int depth, const config_t *cfg) {
  (void)st;
  (void)cfg;
  (void)is_dir;
  print_indent(depth);
  puts(name);
}

static void print_with_S(const char *name, const struct stat *st, int is_dir, int depth, const config_t *cfg) {
  (void)cfg;
  print_indent(depth);

  char pbuf[10];
  perm_str(st->st_mode, pbuf);
  char tbuf[32];
  time_str(st->st_atime, tbuf, sizeof tbuf);
  long long shown = is_dir ? 0LL : (long long)st->st_size;

  printf("%s (size=%lld, perm=%s, atime=%s)\n", name, shown, pbuf, tbuf);
}

static int walk_one(const char *path, int depth, const config_t *cfg, filter_fn f, print_fn p) {
  struct stat st;
  if (lstat(path, &st) != 0) {
    fprintf(stderr, "lstat failed on %s: %s\n", path, strerror(errno));
    return -1;
  }

  const char *base = strrchr(path, '/');
  base = base ? base + 1 : path;
  if (base[0] == '\0') base = "/";

  int is_dir = S_ISDIR(st.st_mode);

  char display[PATH_MAX];
  if (S_ISLNK(st.st_mode)) {
    char target[PATH_MAX];
    ssize_t n = readlink(path, target, sizeof(target) - 1);
    if (n >= 0) { target[n] = '\0'; }
    else { snprintf(target, sizeof target, "?"); }
    snprintf(display, sizeof display, "%s (-> %s)", base, target);
  } else {
    snprintf(display, sizeof display, "%s", base);
  }

  if (f(base, &st, is_dir, depth, cfg)) {
    p(display, &st, is_dir, depth, cfg);
  }

  if (is_dir) {
    if (!cfg->substr || depth < cfg->max_depth) {
      DIR *d = opendir(path);
      if (!d) { fprintf(stderr, "opendir failed on %s: %s\n", path, strerror(errno)); return -1; }
      struct dirent *ent;
      char child[PATH_MAX];
      while ((ent = readdir(d))) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
        int n = snprintf(child, sizeof child, "%s/%s", path, ent->d_name);
        if (n < 0 || n >= (int)sizeof child) {
          fprintf(stderr, "path too long: %s/%s\n", path, ent->d_name);
          continue;
        }
        walk_one(child, depth + 1, cfg, f, p);
      }
      closedir(d);
    }
  }
  return 0;
}

static int walk_and_print(const char *root, const config_t *cfg,
                          filter_fn f, print_fn p) {
  return walk_one(root, 0, cfg, f, p);
}

static void usage(const char *prog) {
  fprintf(stderr,
    "Usage: %s [options] [start_dir]\n"
    "Options:\n"
    "  -S\n"
    "  -s <bytes>\n"
    "  -f <pattern> <d>\n"
    "  -t <f|d>\n",
    prog);
}

int main(int argc, char *argv[]) {
  config_t cfg;
  cfg.show_S    = 0;
  cfg.size_le   = (off_t)-1;
  cfg.substr    = NULL;
  cfg.max_depth = 0;
  cfg.type_only = 0;
  cfg.start_dir = ".";

  int opt;
  while ((opt = getopt(argc, argv, "Ss:f:t:")) != -1) {
    switch (opt) {
      case 'S':
        cfg.show_S = 1; break;
      case 's': {
        char *end = NULL;
        long long v = strtoll(optarg, &end, 10);
        if (end == optarg || v < 0) { usage(argv[0]); return 1; }
        cfg.size_le = (off_t)v;
      } break;
      case 'f':
        cfg.substr = optarg;
        if (optind >= argc) { usage(argv[0]); return 1; }
        cfg.max_depth = atoi(argv[optind]);
        if (cfg.max_depth < 0) { usage(argv[0]); return 1; }
        optind++;
        break;
      case 't':
        if (optarg && (optarg[0] == 'f' || optarg[0] == 'd') && optarg[1] == '\0')
          cfg.type_only = optarg[0];
        else { usage(argv[0]); return 1; }
        break;
      default:
        usage(argv[0]); return 1;
    }
  }
  if (optind < argc) {
    cfg.start_dir = argv[optind];
  }
  print_fn pf = cfg.show_S ? print_with_S : print_basic;
  return (walk_and_print(cfg.start_dir, &cfg, pass_all, pf) == 0) ? 0 : 1;
}