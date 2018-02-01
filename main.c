#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backup/malloc.h"

int main() {
	int j = 0;
/*
	while (j < 10) {
		char *str = malloc(1);
		char *str2 = malloc(1);
		my_put_nbr(str);
		my_putstr("\n");
		my_put_nbr(str2);
		my_putstr("\n");
		free(str);
		free(str2);
		j++;
	}*/
	/*
	char *str = malloc(4);
	for (int i = 0; i < 10000; i++) {
		str[i] = "a";
	}
	str = malloc(6);*/
	/*
	while (1) {
		char *str = strdup("bite en boucle\n");
		char *str2 = strdup("E-Penis\n");
		my_putstr(str);
		my_putstr(str2);
		free(str);
		free(str2);
	}*/
	/*char **str = malloc(sizeof(char *) * 1);

	while (j < 10) {
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
	free(str);*/
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

