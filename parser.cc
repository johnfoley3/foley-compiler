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

void Parser::parse_error (string *expected, Token *found) {

  printf("Syntax error: Expected %s, found %s\n", expected->c_str(), found->to_string()->c_str());
  delete expected;
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

bool Parser::parse_variable_decl_list() {

    // VARIABLE_DECL_LIST -> VARIABLE_DECL ; VARIABLE_DECL_LIST
    //                    -> labmda

    // Predict (VARIABLE_DECL ; VARIABLE_DECL_LIST) => {identifier, procedure, begin}

    // match identifier
    if (word->get_token_type() == TOKEN_ID)  {

        if(parse_variable_decl()) {

            if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                if (parse_variable_decl_list()) {

                    // successfully parsed variable decl list
                    return true;
                }
            } else {

                string *expected = new string ("\";\"");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse variable decl
            return false;
        }
    // PREDICT (VARIABLE_DECL_LIST -> LAMBDA) => FOLLOW(VARAIBLE_DECL_LIST)
    //                                        => {procedure, begin}
    // match procedure or begin
    } else if ((word->get_token_type() == TOKEN_KEYWORD 
    && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE)
        || (word->get_token_type() == TOKEN_KEYWORD
    && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)) {

            // successfully parsed the lambda production
            return true;

    } else {

        string *expected = new string ("identifier, keyword \"begin\" or keyword \"procedure\"");
        parse_error(expected, word);
        return false;
    }

    return false;

}

bool Parser::parse_variable_decl() {

    // VARIABLE_DECL -> IDENTIFIER_LIST : STANDARD_TYPE
    // PREDICT(IDENTIFIER_LIST : STANDARD_TYPE) => {identifier}

    // match identifier
    if (word->get_token_type() == TOKEN_ID) {

        if (parse_identifier_list()) {

            // match colon
            if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                if (parse_standard_type()) {

                    //Successfully parsed variable decl
                    return true;
                } else {

                    // failed to parse standard type
                    return false;
                }
            } else {

                string *expected = new string ("\":\"");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse identifier list
            return false;
        }
    } else {

        string *expected = new string ("identifier");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_procedure_decl_list() {

    //PROCEDURE_DECL_LIST -> PROCEDURE_DECL ; PROCEDURE_DECL_LIST
    //                    -> LAMBDA

    // PREDICT(PROCEDURE_DECL ; PROCEDURE_DECL_LIST) = {procedure}

    // match procedure
    if (word->get_token_type() == TOKEN_KEYWORD 
    && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE) {

        if (parse_procedure_decl()) {

            // Match ;
            if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

                // ADVANCE
                delete word; 
                word = lex->next_token();

                if (parse_procedure_decl_list()) {

                    // successfully parsed procedure_decl_list
                    return true;
                } else {

                    // failed to parse procedure_decl_list
                    return false;
                }
            } else {

                string *expected = new string ("\";\"");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse procedure_decl
            return false;
        }
    // PREDICT(PROCEDURE_DECL_LIST -> LAMBDA) = {begin}
    } else {

        if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN) {

            // successfully parsed lambda
            return true;
        } else {

            string *expected = new string ("keyword \"begin\"");
            parse_error(expected, word);
            return false;
        }
    }

    return false;
}

bool Parser::parse_procedure_decl() {

    // PROCEDURE_DECL -> procedure identifier ( arg_list ) VARIABLE_DECL_LIST BLOCK
    // PREDICT(procedure identifier ( arg_list ) VARIABLE_DECL_LIST BLOCK) => {procedure}

    // match procedure
    if (word->get_token_type() == TOKEN_KEYWORD 
    && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE) {

        // match identifier
        if (word->get_token_type() == TOKEN_ID) {

            // match (
            if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) {

                if (parse_arg_list()) {

                    // match )
                    if (word->get_token_type() == TOKEN_PUNC 
                    && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

                        if (parse_variable_decl_list()) {

                            if (parse_block()) {

                                // successfully parsed procedure_decl
                                return true;
                            } else {

                                // failed to parse block
                                return false;
                            }
                        } else {

                            // failed to parse variable_decl_list
                            return false;
                        }
                    } else {

                        // failed to find )
                        string *expected = new string ("\")\"");
                        parse_error(expected, word);
                        return false;
                    }
                } else {

                    // failed to parse arg_list
                    return false;
                }
            } else {

                // failed to find (
                string *expected = new string ("\"(\"");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to find identifier
            string *expected = new string ("identifier");
            parse_error(expected, word);
            return false;
        }
    } else {

        // failed to find procedure
        string *expected = new string ("keyword \"procedure\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_arg_list() {

    // ARG_LIST -> IDENTIFIER_LIST : STANDARD_TYPE ARG_LIST_HAT
    //          -> LAMDA
    // PREDICT(IDENTIFIER_LIST : STANDARD_TYPE ARG_LIST_HAT) => {identifier}

    // match identifier
    if (word->get_token_type() == TOKEN_ID) {

        if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON) {

            if (parse_standard_type()) {

                if (parse_arg_list_hat()) {

                    // successfully parsed arg_list
                    return true;
                } else {

                    // failed to parse arg_list_hat
                    return false;
                }
            } else {

                // failed to parse standard type
                return false;
            }
        } else {

            // failed to find :
            string *expected = new string ("\":\"");
            parse_error(expected, word);
            return false;
        }
    // PREDICT(ARGS_LIST -> LAMBDA) => { ) }
    // match )
    } else if (word->get_token_type() == TOKEN_PUNC 
                    && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

        // successfully parsed lambda
        return true;
    } else {

        // failed to find identifier or )
        string *expected = new string ("identifier or \")\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_arg_list_hat() {

    // ARG_LIST_HAT -> ; ARG_LIST
    //              -> LAMBDA
    // PREDICT(; ARG_LIST) => { ; }

    // match ;
    if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

        if (parse_arg_list()) {

            // successfully parsed arg_list_hat
            return true;
        } else {

            // failed to parse arg_list 
            return false;
        }
    } else if (word->get_token_type() == TOKEN_PUNC 
                    && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

        // successfully parsed lambda
        return true;

    } else {

        // failed to find ; or )
        string *expected = new string ("\";\" or \")\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_identifier_list() {

    return false;
}

bool Parser::parse_standard_type() {

    return false;
}

bool Parser::parse_block() {

    // BLOCK -> 
    return true;
}
