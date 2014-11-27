#include "parser.h"
#include <stdio.h>

Parser::Parser(Scanner *s) 
{
  lex = s;
  // Init the word variable.
  word = lex->next_token();
}

Parser::~Parser() 
{

  if (lex != NULL) {

    delete lex;
  }

  if (word != NULL) {

    delete word;
  }
}

void parse_error (string *expected, Token *found) {

  printf("Syntax error: Expected %s, found %s\n", expected, found);
}

// If we have parsed the entire program, then word
// should be the EOF Token.  This function tests
// that condition.
bool Parser::done_with_input()
{
  return word->get_token_type() == TOKEN_EOF;
}

bool Parser::parse_program()
{
   // PROGRAM -> program identifier ; DECL_LIST BLOCK ;
   // Predict (program identifier ; DECL_LIST BLOCK ;) == {program}

   // Match keyword program
   if (word->get_token_type() == TOKEN_KEYWORD 
       && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROGRAM) {

     /* ADVANCE  - Notice that we only delete a token on an ADVANCE, and, if
       we ADVANCE, it is the ADVANCE code that is responsible for 
       getting the next token.
     */
     delete word; 
     word = lex->next_token();

     // Match identifier
     if (word->get_token_type() == TOKEN_ID) {		
        // ADVANCE
        delete word; 
        word = lex->next_token();
	// Match ;
	if (word->get_token_type() == TOKEN_PUNC 
	    && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
	  // ADVANCE
	  delete word; 
	  word = lex->next_token();

	  // Match DECL_LIST - ACTION
	  if (parse_decl_list()) {
	    
	    // Match BLOCK - ACTION
	    if (parse_block()) {
	      
	      if (word->get_token_type() == TOKEN_PUNC
		  && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
		// ADVANCE
		delete word;
		word = lex->next_token();
		// Parsing of non-terminal PROGRAM succeeded
		return true;
		
		// We failed to match the second semicolon
	      } else {
		string *expected = new string ("';'");
		// Expected should be deleted in parse_error()
		parse_error (expected, word);

		return false;
	      }
	      
	      // We failed to parse BLOCK
	    } else {

	      return false;
	    }
	    
	    // We failed to parse DECL_LIST
	  } else {

	    return false;
	  }
	  
	  // We failed to match the first semicolon
	} else {
	  string *expected = new string ("';'");
	  parse_error (expected, word);
	  return false;
	}
	
	// We failed to match an identifier
     } else {
       string *expected = new string ("identifier");
       parse_error (expected, word);
       return false;
     }
     
     // We failed to match the keyword program
   } else {
     string *expected = new string ("program");
     parse_error (expected, word);
     return false;
   }
   
   // We shouldn't reach this statement.
   return false;
}

bool Parser::parse_decl_list()
{
   /* DECL_LIST -> VARIABLE_DECL_LIST PROCEDURE_DECL_LIST
    * Predict (VARIABLE_DECL_LIST PROCEDURE_DECL_LIST) = 
    *          First (VARIABLE_DECL_LIST)
    *    union First (PROCEDURE_DECL_LIST)
    *    union Follow (DECL_LIST) = {identifier, procedure, begin}
    */
   if (word->get_token_type() == TOKEN_ID 
       || (word->get_token_type() == TOKEN_KEYWORD 
	  && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE)
       || (word->get_token_type() == TOKEN_KEYWORD
	   && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)) {

     if (parse_variable_decl_list()) {  // ACTION

        if (parse_procedure_decl_list()) { // ACTION
	   return true;
	} else {
	   // parse_procedure_decl_list() failed
	  return false;
	}

      
      } else {
         // parse_variable_decl_list() failed
         return false;
      }

   } else {
      // word isn't in predict (DECL_LIST)
      string *expected = new string ("identifier, keyword \"begin\" or keyword \"procedure\"");
      parse_error (expected, word);
      return false;
   }

}

bool Parser::parse_block() {

    return true;
}

bool Parser::parse_variable_decl_list() {

    return true;
}

bool Parser::parse_procedure_decl_list() {

    return true;
}
