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

        // ADVANCE
        delete word;
        word = lex->next_token();

        // match identifier
        if (word->get_token_type() == TOKEN_ID) {

            // ADVANCE
            delete word;
            word = lex->next_token();

            // match (
            if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                if (parse_arg_list()) {

                    // match )
                    if (word->get_token_type() == TOKEN_PUNC 
                    && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

                        // ADVANCE
                        delete word;
                        word = lex->next_token();

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

            // ADVANCE
            delete word;
            word = lex->next_token();

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

        // ADVANCE
        delete word;
        word = lex->next_token();

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

    // IDENTIFIER_LIST -> identifier IDENTIFIER_LIST_PRM
    // PREDICT(identifier IDENTIFIER_LIST_PRM) => {identifier}

    //match identifier
    if (word->get_token_type() == TOKEN_ID) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_identifier_list_prm()) {

            // successfully parsed identifier_list
            return true;
        } else {

            // failed to parse identifier_list
            return false;
        }
    } else {

        // failed to find identifier
        string *expected = new string ("identifier");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_identifier_list_prm() {

    //IDENTIFIER_LIST_PRM -> , identifier IDENTIFIER_LIST_PRM
    //                    -> LAMBDA
    // PREDICT(, identifier IDENTIFIER_LIST_PRM) => {,}

    // match ,
    if (word->get_token_type() == TOKEN_PUNC 
                    && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        // match identifier
        if (word->get_token_type() == TOKEN_ID) {

            // ADVANCE
            delete word;
            word = lex->next_token();

            if (parse_identifier_list_prm()) {

                //successfully parsed identifier_list_prm
                return true;
            } else {

                // identifier_list_prm failed to parse
                return false;
            }
        } else {

            // failed to find identifier
            string *expected = new string ("identifier");
            parse_error(expected, word);
            return false;
        }

        //predict (IDENTIFIER_LIST_PRM -> LAMBDA) => {:}
    } else if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON) {

        // successfully parsed lambda closure
        return true;
    } else {

        // failed to find , or :
        string *expected = new string ("\",\" or \":\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_standard_type() {

    // STANDARD_TYPE -> int
    //               -> bool

    // match int
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_INT) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        // successfully parsed int
        return true;

        // match bool
    } else if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_BOOL) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        // successfully parsed bool
        return true;
    } else {

        // failed to find num or bool
        string *expected = new string ("number or bool");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_block() {

    // BLOCK -> begin STMT_LIST end
    // PREDICT(begin STMT_LIST end)

    // match begin
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_stmt_list()) {

            // match end
            if (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_END) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                // successfully parsed block
                return true;
            } else {

                // failed to find end
                string *expected = new string ("end");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse stmt_list
            return false;
        }
    } else {

        // failed to find begin
        string *expected = new string ("begin");
        parse_error(expected, word);
        return false;
    }
    return true;
}

bool Parser::parse_stmt_list() {

    // STMT_LIST -> STMT ; STMT_LIST_PRM
    // Predict(STMT ; STMT_LIST_PRM) => {if, while, print, identifier}

    // match if or while or print or identifier
    if ((word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
        || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
        || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)
        || (word->get_token_type() == TOKEN_ID)) {

        if (parse_stmt()) {

            // match ;
            if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                if (parse_stmt_list_prm()) {

                    // successfully parsed stmt_list
                    return true;
                } else {

                    // failed to parse stmt_list
                    return false;
                }
            } else {

                // failed to find begin
                string *expected = new string ("\";\"");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse stmt
            return false;
        }
    } else {

        // failed to find if, while, print, identifier
        string *expected = new string ("if, while, print, identifier");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_stmt_list_prm() {

    // STMT_LIST_PRM -> STMT ; STMT_LIST_PRM
    //               -> LAMBDA
    // Predict(STMT ; STMT_LIST_PRM) => {if, while, print, identifier}

    // match if or while or print or identifier
    if ((word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
        || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
        || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)
        || (word->get_token_type() == TOKEN_ID)) {

        if (parse_stmt()) {

            // match ;
            if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                if (parse_stmt_list_prm()) {

                    // successfully parsed stmt_list
                    return true;
                } else {

                    // failed to parse stmt_list
                    return false;
                }
            } else {

                // failed to find begin
                string *expected = new string ("\";\"");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse stmt
            return false;
        }
        // predict(STMT_LIST_PRM -> lambda) = {end}
        // match end
    } else if (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_END) {

        // successfully parsed lambda production
        return true;
    } else {

        // failed to find if, while, print, identifier, end
        string *expected = new string ("if, while, print, identifier, end");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_stmt() {

    // STMT -> IF_STMT
    //      -> WHILE_STMT
    //      -> PRINT_STMT
    //      -> identifier STMT_ASS_PROC_TAIL
    // Predict(IF_STMT) => {if}

    // match if
    if (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF) {

        if (parse_if_stmt()) {

            // successfully parsed stmt
            return true;
        } else {

            // failed to parse if_stmt
            return false;
        }

        // Predict(WHILE_STMT) => {while}
        // match while
    } else if (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE) {

        if (parse_while_stmt()) {

            // successfully parsed stmt
            return true;
        } else {

            // failed to parse parse_while_stmt
            return false;
        }
        // Predict(WHILE_STMT) => {while}
        // match print
    } else if (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT) {

        if (parse_print_stmt()) {

            // successfully parsed stmt
            return true;
        } else {

            // failed to parse parse_print_stmt
            return false;
        }
        // Predict(identifier STMT_ASS_PROC_TAIL) => {identifier}
        // match identifier
    } else if (word->get_token_type() == TOKEN_ID) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_stmt_ass_proc_tail()) {

            // successfully parsed stmt
            return true;
        } else {

            // failed to parse parse_stmt_ass_proc_tail
            return false;
        }
    } else {

        // failed to find if, while, print, identifier
        string *expected = new string ("if, while, print, identifier");
        parse_error(expected, word);
        return false;
    }
    return false;
}

bool Parser::parse_stmt_ass_proc_tail() {

    // STMT_ASS_PROC_TAIL -> ASSIGNMENT_STMT_TAIL
    //                    -> PROCEDURE_CALL_STMT_TAIL
    // PREDICT(ASSIGNMENT_STMT_TAIL) => {:=}

    // match :=
    if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_ASSIGN) {

        if (parse_assignment_stmt_tail()) {

            // successfully parsed stmt_ass_proc_tail
            return true;
        } else {

            // failed to parse assignment_stmt_tail
            return false;
        }
        // PREDICT(PROCEDURE_CALL_STMT_TAIL) => {(}
        // match (
    } else if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) {

        if (parse_procedure_call_stmt_tail()) {

            // successfully parsed stmt_ass_proc_tail
            return true;
        } else {

            // failed to parse procedure_call_stmt_tail
            return false;
        }
    } else {

        // failed to find := or (
        string *expected = new string ("\":=\" or \"(\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_assignment_stmt_tail() {

    // ASSIGNMENT_STMT_TAIL -> := EXPR
    // PREDICT(:= EXPR) => {:=}

    // match :=
    if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_ASSIGN) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr()) {

            //successfully parsed assignment_stmt_tail
            return true;
        } else {

            // failed to parse expr
            return false;
        }
    } else {

        // failed to find :=
        string *expected = new string ("\":=\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_if_stmt() {

    // IF_STMT -> if EXPR then BLOCK IF_STMT_HAT
    // PREDICT(if EXPR then BLOCK IF_STMT_HAT) => {if}

    // match if
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr()) {

            if (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                if (parse_block()) {

                    if (parse_if_stmt_hat()) {

                        //successfully parsed if_stmt
                        return true;
                    } else {

                        // failed to parse stmt_hat
                        return false;
                    }
                } else {

                    // failed to parse block
                    return false;
                }
            } else {

                // failed to find if
                string *expected = new string ("if");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse expr
            return false;
        }
    } else {

        // failed to find if
        string *expected = new string ("if");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_if_stmt_hat() {

    // IF_STMT_HAT -> else BLOCK
    //             -> LAMBDA
    // PREDICT(else BLOCK) => {else}

    //match else
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_ELSE) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_block()) {

            //successfully parsed if_stmt_hat
            return true;
        } else {

            //failed to parse block
            return false;
        }
        // PREDICT(IF_STMT_HAT -> LAMBDA) => {;}
    //match ;
    } else if (word->get_token_type() == TOKEN_PUNC 
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

        // successfully parsed lambda production
        return true;
    } else {

        // failed to find else or ;
        string *expected = new string ("else or \";\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_while_stmt() {

    // WHILE_STMT -> while EXPR BLOCK
    // PREDICT(while EXPR BLOCK) => {while}

    // match while
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr()) {

            if (parse_block()) {

                //successfully parsed while_stmt
                return true;
            } else {

                // failed to parse block
                return false;
            }
        } else {

            // failed to parse expr
            return false;
        }
    } else {

        // failed to find while
        string *expected = new string ("while");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_print_stmt() {

    // PRINT_STMT -> print EXPR
    // PREDICT(print EXPR) => {print}

    // match print
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr()) {

            //successfully parsed print_stmt
            return true;
        } else {

            // failed to parse expr
            return false;
        }
    } else {

        // failed to find print
        string *expected = new string ("print");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_procedure_call_stmt_tail() {

    return false;
}

bool Parser::parse_expr() {

    return false;
}
