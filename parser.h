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

#include "symbol_table.h"

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
      bool parse_standard_type(expr_type &standard_type_type);
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
      bool parse_stmt_ass_proc_tail(expr_type &stmt_ass_proc_tail_type);
      bool parse_assignment_stmt_tail(expr_type &assignment_stmt_tail_type);
      bool parse_procedure_call_stmt_tail();
      bool parse_if_stmt_hat();
      bool parse_expr(expr_type &the_expr_type);
      bool parse_expr_list();
      bool parse_expr_list_hat();
      bool parse_simple_expr(expr_type &simple_expr_type);
      bool parse_expr_hat(expr_type &expr_hat_type);
      bool parse_simple_expr_prm(expr_type &simple_expr_prm_type);
      bool parse_term(expr_type &term_type);
      bool parse_term_prm(expr_type &term_prm_type);
      bool parse_factor(expr_type &factor_type);
      bool parse_sign();
  
      // The lexical analyzer
      Scanner *lex;
      // The current token the parser is looking at
      Token *word;

      Symbol_Table *stab;
  
      // Print out a parse error message:  "Expected" expected,
      // "found", found.  This method should delete the string
      // expected.
      void parse_error (string *expected, Token *found);

      // The current environment that we are doing analysis in
      string *current_env;

      // The main environment (the name of the program)
      string *main_env;

      // Used in analysis for position of parameters
      int parm_pos;

      string *expected;

      // Throws the type error
      void type_error(Token *where);

      // Throws the undeclared id error
      void undeclared_id_error(string *id, string *env);
};

#endif
