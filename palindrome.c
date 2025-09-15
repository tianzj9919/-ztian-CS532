#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool isPalindrome(char *str) {
	int i, j, length;

	length = strlen(str);
	for (i = 0, j = length - 1; i < length/2; i++, j--) 
		if (str[i] != str[j]) 
			return false;

	return true;
}

int main(int argc, char** argv) {
	char str[256];

	printf("Enter a string: ");
	scanf("%s", str);

	if (isPalindrome(str))
		printf("%s is a palindrome\n", str);
	else
		printf("%s is not a palindrome\n", str);

	return 0;
}
