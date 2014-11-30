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
      bool parse_identifier_list_prm();
      bool parse_stmt_list();
      bool parse_stmt_list_prm();
      bool parse_stmt();
      bool parse_if_stmt();
      bool parse_while_stmt();
      bool parse_print_stmt();
      bool parse_stmt_ass_proc_tail();
      bool parse_assignment_stmt_tail();
      bool parse_procedure_call_stmt_tail();
      bool parse_if_stmt_hat();
      bool parse_expr();
      bool parse_expr_list();
      bool parse_expr_list_hat();
      bool parse_simple_expr();
      bool parse_expr_hat();
  
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
