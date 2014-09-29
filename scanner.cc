/*
 * Author: John Foley
 * Date: 9.28.14
 * Last Edit: 9.28.14
 */

#include "scanner.h"

Scanner::Scanner ( char *filename) {

	buf = new Buffer( filename );
}

Scanner::~Scanner() {

	delete buf;
}

Token *next_token() {
`
	cout << "from next_token" << endl;
}

void scanner_fatal_error() {

	// My attempt to be funny at 2 in the morning	
	cout << "Uh oh. Scanner ran into a fatel problem. Goodluck." << endl;
	exit(0);
}
