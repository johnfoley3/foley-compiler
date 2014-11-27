/*
Author: John Foley

Date: 11.27.14
Last Edit: 11.27.14
*/

#include <stdio.h>

int main(int argc, const char* argv[]) {

	if (argc < 0 || argc > 1) {

		printf("Expected one argument, the input file name\n");
	}

	const char* filename = argv[0];

	printf("%s \n", filename);
}
