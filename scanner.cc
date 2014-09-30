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
	string *attr = new string("");
	bool done = false;
	Token *token;

	while (!done) {

		char c = buf->next_char();
		
		switch (state) {
			
			// This is the big guy. What does our next hopefully legal lexeme start with?
			case 0:

				// check for every initial transition
				if (c == 'p') {

					// go to state for 'pr'
					state = 1;
					*attr += c;
				} else if (c == 'n') {
					
				} else if (c == 'i') {
					
				} else if (c == 'b') {
					
				} else if (c == 't') {
					
				} else if (c == 'e') {
					
				} else if (c == 'w') {
					
				} else if (c == ';') {
					
				} else if (c == ':') {
					
				} else if (c == ',') {
						
				} else if (c == '(') {
					
				} else if (c == ')') {

				} else if (c == '=') {
					
				} else if (c == '<') {
					
				} else if (c == '>') {
				
				} else if (c == '+') {
					
				} else if (c == 'o') {
					
				} else if (c == '*') {
					
				} else if (c == 'a') {
					
				} else if ( is_alpha(c) ) {
					
				} else if ( is_digit(c) ) {
					
				} else  if (c == '$') {
					
					// We made it to the end!! Someone grab some champagne!
					token = new EofToken();
					done = true;
				} else {

					scanner_fatal_error();
					exit(0);
				}
				break;
			
			case 1:

				if (c == 'r') {

					// 'pr'
					state = 2;
					 *attr += c;	
				} else if ( is_alphanumeric(c) ) {
					
					// we have a possible identifier on our hands
					state = 8;
					 *attr += c;
				} else if ( is_symbol(c)) {
					
					token = new IdToken(attr);
					done = true;
					buf->unread_char(c);
				} else if ( c == ' ') {
					
					token = new IdToken(attr);
					done = true;
				} else {
		
					scanner_fatal_error();
					exit(0);
				}
				break;

			case 2: 
				
				if (c == 'o') {

					// 'pro'
					state = 3;
					 *attr += c;
				} else if ( is_alphanumeric(c) ) {
					
					state = 8;
					 *attr += c;
				} else if ( is_symbol(c) ) {
					
					token = new IdToken(attr);
					done = true;
					buf->unread_char(c);
				} else if ( c == ' ') {
					
					token = new IdToken(attr);
					done = true;
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;
	
			case 3:

				if (c == 'g') {

					// 'prog'
					state = 4;
					 *attr += c;
				} else if ( is_alphanumeric(c) )  {
		
					state = 8;
					 *attr += c;
				} else if ( is_symbol(c) ) {
					
					token = new IdToken(attr);
					done = true;
					buf->unread_char(c);
				} else if ( c == ' ') {
					
					token = new IdToken(attr);
					done = true;
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;

			case 4: 
		
				if (c == 'r') {

					// 'progr'
					state = 5;
					 *attr += c;
				} else if ( is_alphanumeric(c) ) {

					state = 8;
					 *attr += c;
				} else if ( is_symbol(c) ) {
					
					token = new IdToken(attr);
					done = true;
					buf->unread_char(c);
				} else if ( c == ' ') {
					
					token = new IdToken(attr);
					done = true;
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;

			case 5:

				if (c == 'a') {

					// 'progra'
					state = 6;
					 *attr += c;
				} else if ( is_alphanumeric(c) ) {

					state = 8;
					 *attr += c;
				} else if ( is_symbol(c) ) {
					
					token = new IdToken(attr);
					done = true;
					buf->unread_char(c);
				} else if ( c == ' ') {
					
					token = new IdToken(attr);
					done = true;
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;

			case 6: 
	
				if (c == 'm') {

					// 'program'
					state = 7;
					 *attr += c;
				} else if ( is_alphanumeric(c) ) {

					state = 8;
					 *attr += c;
				} else if ( is_symbol(c) ) {
					
					token = new IdToken(attr);
					done = true;
					buf->unread_char(c);
				} else if ( c == ' ') {
					
					token = new IdToken(attr);
					done = true;
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;

			case 7: 

				if ( is_alphanumeric(c) ) {

					state = 8;
					 *attr += c;
				} else if (c == ' ') {

					// We found program!
					token = new KeywordToken(KW_PROGRAM);
					done = true;
				} else if ( is_symbol(c) ) {

					token = new KeywordToken(KW_PROGRAM);
					done = true;
					buf->unread_char(c);
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;


			case 8: 

				if ( is_alphanumeric(c) ) {

					state = 8;
					 *attr += c;
				} else if (c == ' ') {

					// We found program!
					token = new IdToken(attr);
					done = true;
				} else if ( is_symbol(c) ) {

					token = new IdToken(attr);
					done = true;
					buf->unread_char(c);
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;

		}
	}

	return token;
}

void Scanner::scanner_fatal_error() {

	// My attempt to be funny at 2 in the morning	
	cout << "Uh oh. Scanner ran into a fatel problem. Goodluck." << endl;
	exit(0);
}
