#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"

int main() {
	int j = 0;
	my_putstr("\n");show_alloc_mem();
	void *ptr1 = malloc(1);
	my_putstr("\n");show_alloc_mem();
	void *ptr2 = malloc(2);
	my_putstr("\n");show_alloc_mem();
	void *ptr3 = malloc(3);
	my_putstr("\n");show_alloc_mem();
	void *ptr4 = malloc(4);
	my_putstr("\n");show_alloc_mem();
	void *ptr5 = malloc(5);
	my_putstr("\n");show_alloc_mem();
	void *ptr6 = malloc(6);
	my_putstr("\n");show_alloc_mem();
	void *ptr7 = malloc(7);
	my_putstr("\n");show_alloc_mem();
	void *ptr8 = malloc(8);
	my_putstr("\n");show_alloc_mem();
	void *ptr9 = malloc(9);
	my_putstr("\n");show_alloc_mem();
	free(ptr7);
	my_putstr("\n");show_alloc_mem();
	free(ptr4);
	my_putstr("\n");show_alloc_mem();
	free(ptr5);
	my_putstr("\n");show_alloc_mem();
	free(ptr6);
	my_putstr("\n");show_alloc_mem();
	free(ptr1);
	my_putstr("\n");show_alloc_mem();
	free(ptr2);
	my_putstr("\n");show_alloc_mem();
	free(ptr3);
	my_putstr("\n");show_alloc_mem();
	free(ptr8);
	my_putstr("\n");show_alloc_mem();
	ptr9 = realloc(ptr9, 160000);
	my_putstr("\n");show_alloc_mem();

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

