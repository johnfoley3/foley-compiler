/*
Author: John Foley

Date: 11.27.14
Last Edit: 11.27.14
*/

#include <stdio.h>

int main(int argc, const char* argv[]) {

	if (argc < 0 || argc > 2) {

		printf("Expected one argument, the input file name\n");
	}

	const char* filename = argv[1];

	printf("%s \n", filename);
}
