#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "keywordtoken.h"
#include "punctoken.h"
#include "reloptoken.h"
#include "addoptoken.h"
#include "muloptoken.h"
#include "idtoken.h"
#include "numtoken.h"
#include "eoftoken.h"
#include "scanner.h"

#include <iostream>

using namespace std;

class Parser {
   public:
      Parser (Scanner *s);
      ~Parser();
  
      bool parse_program();
  
   private:
      bool parse_decl_list();
      // etc, etc...; one for each non-terminal in TruPL 2.0
  
      // Return true if current word is the EOF token
      bool done_with_input();
  
      // The lexical analyzer
      Scanner *lex;
      // The current token the parser is looking at
      Token *word;
  
      // Print out a parse error message:  "Expected" expected,
      // "found", found.  This method should delete the string
      // expected.
      void parse_error (string *expected, Token *found);

      // Other helper functions that you may define
  
};

#endif