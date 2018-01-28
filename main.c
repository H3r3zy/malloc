#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backup/malloc.h"

int main() {
	int j = 0;
	/*
	while (1) {
		char *str = malloc(1024);
		char *str2 = malloc(1024);
		printf("%p\n", str, str);
		printf("%p\n", str2, str2);
		free(str);
		free(str2);
		j++;
	}*/
	/*while (1) {
		char *str = strdup("bite en boucle");

		printf("%p\n", get_memory_block_by_data_ptr(str));
		char *str2 = strdup("E-Penis");
		printf("%p\n", get_memory_block_by_data_ptr(str2));
		free(str);
		free(str2);
	}*/
	char **str = malloc(sizeof(char *) * 1);
	while (j < 10000) {
		str[j] = strdup("E-Penis");
		printf("%s :: %p :: ", str[j], str[j]);
		str = realloc(str, sizeof(char *) * (j + 2));
		printf("%s :: %p\n", str[j], str[j]);
		j++;
	}
	str[j] = NULL;
	j = 0;
	while (str[j] != NULL) {
		printf("%s\n", str[j]);
		free(str[j]);
		j++;
	}
	free(str);
	/*
	char **str = malloc(sizeof(char *) * 2);
	while (j < 1) {
		str[j] = strdup("E-Penis");
		printf("B :: %p :: %p :: %s\n", str, str[j], str[j]);
		str = realloc(str, (j + 3) * sizeof(char *));
		printf("A :: %p", str);
		printf(" :: %p :: %s\n", str[j], str[j]);

		j++;
	}*/
	//free(str);
	/*while (1) {
		char *str = strdup("E-Penis");
		char *str2 = strdup("E-Penis");

		printf("###%p\n", str);
		printf("###%p\n", str2);
		free(str2);
		free(str);

	}*/
}

