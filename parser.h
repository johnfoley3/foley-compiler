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

      // Return true if current word is the EOF token
      bool done_with_input();
  
   private:
      bool parse_decl_list();
      bool parse_block();
      bool parse_variable_decl_list();
      bool parse_procedure_decl_list();
      bool parse_variable_decl();
      bool parse_identifier_list();
      bool parse_standard_type();
      bool parse_procedure_decl();
      bool parse_arg_list();
      bool parse_arg_list_hat();
  
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
