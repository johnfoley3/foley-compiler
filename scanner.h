/*
Author: John Foley
URL: thisfoley.ninja
Template by: Dr. Bob Matthews

Date: 9.25.14
Last Edit: 9.25.14
*/

#ifndef SCANNER_H 
#define SCANNER_H

#include <iostream>
#include <stdlib.h>

// The scanner reads from the buffer.
#include "buffer.h"

// The scanner returns objects from the Token class when 
// next_token() is called.
#include "token.h"
#include "keywordtoken.h"
#include "punctoken.h"
#include "reloptoken.h"
#include "addoptoken.h"
#include "muloptoken.h"
#include "idtoken.h"
#include "numtoken.h"
#include "eoftoken.h"

using namespace std;

class Scanner
{
 public:
  Scanner (char *filename);

  ~Scanner();

  // Return the next token in the file.
  Token *next_token();

 private:

  Buffer *buf;

  // Some helper functions
  inline bool is_symbol (char c) {
	return (c=='(' || c==')' || c=='<' || c=='>' || c=='=' || c==':' ||
		c==';' || c==',' || c=='+' || c=='-' || c=='*' || c=='/');
  }

  inline bool is_alpha (char c)
  {
    return c >= 'a' && c <= 'z';
  }

  inline bool is_digit (char c)
  {
    return c >= '0' && c <= '9';
  }

  inline bool is_alphanumeric (char c)
  {
    return is_alpha(c) || is_digit(c);
  }

  /* If you encounter an error that you can't recover from, print an
     error message and call this function.
   */
  void scanner_fatal_error();

  // Probably lots of other helper functions here to support your
  // implementation of next_token().
};
#endif
  
