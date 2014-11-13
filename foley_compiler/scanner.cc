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
					
					// go to state for 't'
					state = 33;
					*attr += c;
				} else if (c == 'e') {
					
					// go to state for 'e'
					state = 37;
					*attr += c;
				} else if (c == 'w') {
					
					// go to state for 'w'
					state = 43;
					*attr += c;
				} else if (c == ';') {
					
					token = new PuncToken(PUNC_SEMI);
					done = true;
				} else if (c == ':') {

					state = 48;
					*attr += c;					
				} else if (c == ',') {
						
					token = new PuncToken(PUNC_COMMA);
					done = true;
				} else if (c == '(') {
					
					token = new PuncToken(PUNC_OPEN);
					done = true;
				} else if (c == ')') {

					token = new PuncToken(PUNC_CLOSE);
					done = true;
				} else if (c == '=') {
					
					token = new RelopToken(RELOP_EQ);
					done = true;
				} else if (c == '<') {
					
					state = 49;
					*attr += c;					
				} else if (c == '>') {
				
					state = 50;
					*attr += c;					
				} else if (c == '+') {
					
					token = new AddopToken(ADDOP_ADD);
					done = true;
				} else if (c == '-') {
					
					token = new AddopToken(ADDOP_SUB);
					done = true;
				} else if (c == 'o') {
					
					state = 51;
					*attr += c;					
				} else if (c == '*') {
					
					token = new MulopToken(MULOP_MUL);
					done = true;
				} else if (c == '/') {
					
					token = new MulopToken(MULOP_DIV);
					done = true;
				} else if (c == 'a') {
					
					state = 53;
					*attr += c;					
				} else if ( is_alpha(c) ) {
					
					// we have a possible identifier on our hands
					state = 8;
					 *attr += c;
				} else if ( is_digit(c) ) {
					state = 56;	
					*attr += c;
				} else  if (c == '$') {
					
					// We made it to the end!! Someone grab some champagne!
					token = new EofToken();
					done = true;
				} else if (c == ' ') {
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


			case 33:

				if (c == 'h') {

					// 'th'
					state = 34;
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

			case 34:

				if (c == 'e') {

					// 'the'
					state = 35;
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

			case 35:

				if (c == 'n') {

					// 'then'
					state = 36;
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


			case 36:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found then!
                                        token = new KeywordToken(KW_THEN);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_THEN);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 37:

				if (c == 'n') {

					// 'en'
					state = 38;
					 *attr += c;	
				} else if (c == 'l') {

					// el
					state = 40;
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

			case 38:

				if (c == 'd') {

					// 'end'
					state = 39;
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

			case 39:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found end!
                                        token = new KeywordToken(KW_END);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_END);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 40:

				if (c == 's') {

					// 'els'
					state = 41;
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

			case 41:

				if (c == 'e') {

					// 'else'
					state = 42;
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

			case 42:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found else!
                                        token = new KeywordToken(KW_ELSE);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_ELSE);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 43:

				if (c == 'h') {

					// 'wh'
					state = 44;
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

			case 44:

				if (c == 'i') {

					// 'whi'
					state = 45;
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


			case 45:

				if (c == 'l') {

					// 'whil'
					state = 46;
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

			case 46:

				if (c == 'e') {

					// 'while'
					state = 47;
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

			case 47:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found while!
                                        token = new KeywordToken(KW_WHILE);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new KeywordToken(KW_WHILE);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 48:

				if (c == '=') {
					
					token = new PuncToken(PUNC_ASSIGN);
					done = true;
				} else {
					token = new PuncToken(PUNC_COLON);
					done = true;
					buf->unread_char(c);
				}
				break;

			case 49:

				if (c == '>') {
					
					token = new RelopToken(RELOP_NEQ);
					done = true;
				} else if (c == '=') {
					
					token = new RelopToken(RELOP_LE);
					done = true;
				} else {
					token = new RelopToken(RELOP_LT);
					done = true;
					buf->unread_char(c);
				}
				break;

			case 50:

				if (c == '=') {
					
					token = new RelopToken(RELOP_GE);
					done = true;
				} else {
					token = new RelopToken(RELOP_GT);
					done = true;
					buf->unread_char(c);
				}
				break;

			case 51:

				if (c == 'r') {

					// 'or'
					state = 52;
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

			case 52:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found or!
                                        token = new AddopToken(ADDOP_OR);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new AddopToken(ADDOP_OR);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 53:

				if (c == 'n') {

					// 'an'
					state = 54;
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

			case 54:

				if (c == 'd') {

					// 'and'
					state = 55;
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

			case 55:

                                if ( is_alphanumeric(c) ) {

                                        state = 8;
                                         *attr += c;
                                } else if (c == ' ') {

                                        // We found and!
                                        token = new MulopToken(MULOP_AND);
                                        done = true;
                                } else if ( is_symbol(c) ) {

                                        token = new MulopToken(MULOP_AND);
                                        done = true;
                                        buf->unread_char(c);
                                } else {

                                        scanner_fatal_error();
                                        exit(0);
                                }

                                break;

			case 56:

				if ( is_digit(c) ) {
				
					state = 56;
					*attr += c;
				} else if ( is_symbol(c) || is_alpha(c) ) {
			
					token = new NumToken(attr);
					done = true;
					buf->unread_char(c);
				} else if (c == ' ') {
		
					token = new NumToken(attr);
					done = true;
				} else {
					
					scanner_fatal_error();
					exit(0);
				}
		}
	}

	return token;
}

void Scanner::scanner_fatal_error() {

	// My attempt to be funny at 2 in the morning	
	cout << "Uh oh. Scanner ran into a fatal problem. Goodluck." << endl;
	exit(0);
}
