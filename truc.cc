/*
Author: John Foley

Date: 11.27.14
Last Edit: 11.27.14
*/

#include <stdio.h>
#include "parser.h"
#include "scanner.h"

using namespace std;

int main(int argc, const char* argv[]) {

	if (argc < 0 || argc > 2) {

		printf("Expected one argument, the input file name\n");
	}

	char *filename = (char *) argv[1];

	Scanner *scan = new Scanner(filename);

	Parser *parse = new Parser(scan);

	if (parse->parse_program()) {

		if (parse->done_with_input()) {
			printf("Parse of program %s has succeeded, no errors.\n", filename);	
		} else {

			printf("SYNTAX ERROR: unexpected tokens at end of file.\n");
		}
	} else {

		printf("Parse of program %s has failed.\n", filename);
	}

	return 0;
}
