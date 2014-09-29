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

Token *Scanner::next_token() {

	int state = 0;
	string *attr = new string();

	while (!done) {

		char c = buf->next_char();
		
		switch (state) {
			
			case 0:
				if (c == 'p') {
					state = 1;
					break;
				}

		}
	}

	return new IdToken();
}

void Scanner::scanner_fatal_error() {

	// My attempt to be funny at 2 in the morning	
	cout << "Uh oh. Scanner ran into a fatel problem. Goodluck." << endl;
	exit(0);
}
