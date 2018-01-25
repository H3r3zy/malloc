#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	while (1) {
		unsigned int i = 0;
		char **str = malloc(sizeof(char *) * 9);
		while (i < 9) {
			str[i] = strdup("bite en boucle");
			i++;
		}
		i = 0;
		while (i < 9) {
			printf("free str[%i]: %s\n", i, str[i]);
			free(str[i]);
			i++;
		}
		free(str);
	}
}
