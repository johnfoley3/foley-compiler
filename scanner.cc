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

					// go to state for 'p'
					state = 1;
					*attr += c;
				} else if (c == 'n') {

					// go to state for 'n'
					state = 18;
					*attr += c;
				} else if (c == 'i') {
					
					// go to state for 'i'
					state = 21;
					*attr += c;
				} else if (c == 'b') {
					
					// go to state for 'b'
					state = 25;
					*attr += c;
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
				} else if (c == 'i') {

					// 'pri'
					state = 9;
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
				} else if (c == 'c') {

					// 'proc'
					state = 12;
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

					// We found an identifier!
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

			case 9: 
		
				if (c == 'n') {

					// 'prin'
					state = 10;
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

			
			case 10: 
		
				if (c == 't') {

					// 'print'
					state = 11;
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

			case 11: 

				if ( is_alphanumeric(c) ) {

					state = 8;
					 *attr += c;
				} else if (c == ' ') {

					// We found print!
					token = new KeywordToken(KW_PRINT);
					done = true;
				} else if ( is_symbol(c) ) {

					token = new KeywordToken(KW_PRINT);
					done = true;
					buf->unread_char(c);
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;

			case 12: 
		
				if (c == 'e') {

					// 'proce'
					state = 13;
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

			case 13: 
		
				if (c == 'd') {

					// 'proced'
					state = 14;
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

			case 14: 
		
				if (c == 'u') {

					// 'procedu'
					state = 15;
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

			case 15: 
		
				if (c == 'r') {

					// 'procedur'
					state = 16;
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


			case 16: 
		
				if (c == 'e') {

					// 'procedure'
					state = 17;
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

			case 17: 

				if ( is_alphanumeric(c) ) {

					state = 8;
					 *attr += c;
				} else if (c == ' ') {

					// We found procedure!
					token = new KeywordToken(KW_PROCEDURE);
					done = true;
				} else if ( is_symbol(c) ) {

					token = new KeywordToken(KW_PROCEDURE);
					done = true;
					buf->unread_char(c);
				} else {

					scanner_fatal_error();
					exit(0);
				}

				break;

			case 18: 
		
				if (c == 'o') {

					// 'no'
					state = 19;
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



			case 19: 
		
				if (c == 't') {

					// 'not'
					state = 20;
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
				
			case 20:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found not!
                                        token = new KeywordToken(KW_NOT);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_NOT);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 21:

				if (c == 'n') {

					// 'in'
					state = 22;
					 *attr += c;	
				} else if (c == 'f') {

					// 'if'
					state = 24;
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


			case 22:

				if (c == 't') {

					// 'int'
					state = 23;
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

			case 23:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found int!
                                        token = new KeywordToken(KW_INT);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_INT);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 24:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found if!
                                        token = new KeywordToken(KW_IF);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_IF);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;


			case 25:

				if (c == 'o') {

					// 'bo'
					state = 26;
					 *attr += c;	
				} else if (c == 'e') {

					// 'be'
					state = 29;
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

			case 26:

				if (c == 'o') {

					// 'boo'
					state = 27;
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

			case 27:

				if (c == 'l') {

					// 'bool'
					state = 28;
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

			case 28:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found bool!
                                        token = new KeywordToken(KW_BOOL);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_BOOL);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 29:

				if (c == 'g') {

					// 'beg'
					state = 30;
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

			case 30:

				if (c == 'i') {

					// 'begi'
					state = 31;
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

			case 31:

				if (c == 'n') {

					// 'begin'
					state = 32;
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

			case 32:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found begin!
                                        token = new KeywordToken(KW_BEGIN);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_BEGIN);
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
