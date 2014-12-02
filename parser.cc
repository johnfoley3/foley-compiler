#include "parser.h"
#include <stdio.h>

Parser::Parser(Scanner *s) {

    lex = s;

    // Init the word variable.
    word = lex->next_token();

    // Symbol table
    stab = new Symbol_Table();

    // Init the environments to null
    current_env = NULL;
    main_env = NULL;
    parm_pos = -1;
}

Parser::~Parser() {

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

void Parser::type_error(Token *where) {

    printf("Type error at %s\n", where->to_string()->c_str());
    exit(0);
}

void Parser::undeclared_id_error(string *id, string *env) {

    printf("%s has not been declared in the scope %s\n", id->c_str(), env->c_str());
    exit(0);
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

            stab->install(static_cast<IdToken *>(word)->get_attribute(), new string("_EXTERNAL"), PROGRAM_T);

            // We only need to set this once.
            main_env = static_cast<IdToken *>(word)->get_attribute();

            current_env = static_cast<IdToken *>(word)->get_attribute();

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

    expr_type standard_type_type;

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

                if (parse_standard_type(standard_type_type)) {

                    stab->update_type(standard_type_type);

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

            stab->install(static_cast<IdToken *>(word)->get_attribute(), current_env, PROCEDURE_T);

            current_env = static_cast<IdToken *>(word)->get_attribute();

            parm_pos = 0;

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

                                current_env = main_env;

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

    expr_type standard_type_type;

    // ARG_LIST -> IDENTIFIER_LIST : STANDARD_TYPE ARG_LIST_HAT
    //          -> LAMDA
    // PREDICT(IDENTIFIER_LIST : STANDARD_TYPE ARG_LIST_HAT) => {identifier}

    // match identifier
    if (word->get_token_type() == TOKEN_ID) {

        if (parse_identifier_list()) {

            if (word->get_token_type() == TOKEN_PUNC
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON) {

                // ADVANCE
                delete word;
                word = lex->next_token();

                if (parse_standard_type(standard_type_type)) {

                    stab->update_type(standard_type_type);

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
        } else {

            // failed to parse identifier_list
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

        // install the new variable to the stab
        stab->install(static_cast<IdToken *>(word)->get_attribute(),
                        current_env, UNKNOWN_T);

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

            // install the new variable to the stab
            stab->install(static_cast<IdToken *>(word)->get_attribute(),
                            current_env, UNKNOWN_T);

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

bool Parser::parse_standard_type(expr_type &standard_type_type) {

    // STANDARD_TYPE -> int
    //               -> bool

    // match int
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_INT) {

        standard_type_type = INT_T;

        // ADVANCE
        delete word;
        word = lex->next_token();

        // successfully parsed int
        return true;

        // match bool
    } else if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_BOOL) {

        standard_type_type = BOOL_T;

        // ADVANCE
        delete word;
        word = lex->next_token();

        // successfully parsed bool
        return true;
    } else {

        // failed to find int or bool
        string *expected = new string ("int or bool");
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

    expr_type stmt_ass_proc_tail_type;

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

        if (! stab->is_decl(static_cast<IdToken *>(word)->get_attribute(), current_env)) {

            undeclared_id_error(static_cast<IdToken *>(word)->get_attribute(), current_env);
        }

        string *left_side = static_cast<IdToken *>(word)->get_attribute();

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_stmt_ass_proc_tail(stmt_ass_proc_tail_type)) {

            if (stab->get_type(left_side, current_env) != stmt_ass_proc_tail_type) {

                type_error(word);
            }

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

bool Parser::parse_stmt_ass_proc_tail(expr_type &stmt_ass_proc_tail_type) {

    expr_type assignment_stmt_tail_type;

    // STMT_ASS_PROC_TAIL -> ASSIGNMENT_STMT_TAIL
    //                    -> PROCEDURE_CALL_STMT_TAIL
    // PREDICT(ASSIGNMENT_STMT_TAIL) => {:=}

    // match :=
    if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_ASSIGN) {

        if (parse_assignment_stmt_tail(assignment_stmt_tail_type)) {

            stmt_ass_proc_tail_type = assignment_stmt_tail_type;

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

            stmt_ass_proc_tail_type = PROCEDURE_T;

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

bool Parser::parse_assignment_stmt_tail(expr_type &assignment_stmt_tail_type) {

    expr_type the_expr_type;

    // ASSIGNMENT_STMT_TAIL -> := EXPR
    // PREDICT(:= EXPR) => {:=}

    // match :=
    if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_ASSIGN) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr(the_expr_type)) {

            assignment_stmt_tail_type = the_expr_type;

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

    expr_type the_expr_type;

    // IF_STMT -> if EXPR then BLOCK IF_STMT_HAT
    // PREDICT(if EXPR then BLOCK IF_STMT_HAT) => {if}

    // match if
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr(the_expr_type)) {

            if (the_expr_type != BOOL_T) {

                type_error(word);
            }

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

    expr_type the_expr_type;

    // WHILE_STMT -> while EXPR BLOCK
    // PREDICT(while EXPR BLOCK) => {while}

    // match while
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr(the_expr_type)) {

            if (the_expr_type != BOOL_T) {

                type_error(word);
            }

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

    expr_type the_expr_type;

    // PRINT_STMT -> print EXPR
    // PREDICT(print EXPR) => {print}

    // match print
    if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        if (parse_expr(the_expr_type)) {

            if (the_expr_type != INT_T) {

                type_error(word);
            }

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

    // PROCEDURE_CALL_STMT_TAIL -> ( EXPR_LIST )
    // PREDICT( ( EXPR_LIST )) => {(}

    if (word->get_token_type() == TOKEN_PUNC 
        && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) {

        parm_pos = 0;

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_expr_list()) {

            if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

                // ADVANCE
                delete word;
                word = lex->next_token(); 

                // successfully parsed procedure_call_stmt_tail
                return true;
            } else {

                // failed to find )
                string *expected = new string ("\")\"");
                parse_error(expected, word);
                return false;
            }    
        } else {

            // failed to parse expr_list
            return false;
        }
    } else {

        // failed to find (
        string *expected = new string ("\"(\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_expr_list() {

    expr_type the_expr_type;

    // EXPR_LIST -> EXPR EXPR_LIST_HAT
    //           -> LAMBDA
    // PREDICT(EXPR EXPR_LIST_HAT) => {identifier, num, (, not, +, -}
    if ((word->get_token_type() == TOKEN_ID)
        || (word->get_token_type() == TOKEN_NUM)
        || (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
        || (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) {

        if (parse_expr(the_expr_type)) {

            if (the_expr_type != stab->get_type(current_env, parm_pos)) {

                type_error(word);
            }

            parm_pos++;

            if (parse_expr_list_hat()) {

                // successfully parsed expr_list
                return true;
            } else {

                // failed to parse expr_list_hat
                return false;
            }
        } else {

            // failed to parse expr
            return false;
        }
        // PREDICT (EXPR_LIST -> LAMBDA) => { ) }
        // match )
    } else if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

        // successfully parsed lambda closure
        return true;
    } else {

        // failed to find identifier, num, (, not, +, -, )
        string *expected = new string ("identifier, num, \"(\", not, \"+\", \"-\" or \")\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_expr_list_hat() {

    // EXPR_LIST_HAT -> , EXPR_LIST
    //               -> LAMBDA
    // PREDICT(, EXPR_LIST) => {,}

    // match ,
    if (word->get_token_type() == TOKEN_PUNC 
        && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA) {

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_expr_list()) {

            // successfully parsed expr_list_hat
            return true;
        } else {

            // failed to parse expr_list
            return false;
        }
    } else if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

        // successfully parsed lambda transition
        return true;
    } else {

        // failed to find , or )
        string *expected = new string ("\",\" or \")\"");
        parse_error(expected, word);
        return false;
    }


    return false;
}

bool Parser::parse_expr(expr_type &the_expr_type) {

    expr_type simple_expr_type, expr_hat_type;

    // EXPR -> SIMPLE_EXPR EXPR_HAT
    // PREDICT (SIMPLE_EXPR EXPR_HAT) => {identifier, num, (, +, -, not}

    // match identifier, num, (, +, -, not
    if ((word->get_token_type() == TOKEN_ID)
        || (word->get_token_type() == TOKEN_NUM)
        || (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
        || (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) {

        if (parse_simple_expr(simple_expr_type)) {

            if (parse_expr_hat(expr_hat_type)) {

                // calculate the expr_type
                if (expr_hat_type == NO_T) {

                    the_expr_type = simple_expr_type;
                } else if ((simple_expr_type == INT_T) && (expr_hat_type == INT_T)) {

                    the_expr_type = BOOL_T;
                } else {

                    type_error(word);
                }

                // successfully parsed expr
                return true;
            } else {

                // failed to parse expr_hat
                return false;
            }
        } else {

            // failed to parse simple expr
            return false;
        }
    } else {

        // failed to find identifier, num, (, not, +, -
        string *expected = new string ("identifier, num, \"(\", not, \"+\" or \"-\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_simple_expr(expr_type &simple_expr_type) {

    expr_type term_type, simple_expr_prm_type;

    // SIMPLE_EXPR -> TERM SIMPLE_EXPR_PRM
    // PREDICT(TERM SIMPLE_EXPR_PRM) => {identifier, num, (, +, -, not}

    // match identifier, num, (, +, -, not
    if ((word->get_token_type() == TOKEN_ID)
        || (word->get_token_type() == TOKEN_NUM)
        || (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
        || (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) {

        if (parse_term(term_type)) {

            if (parse_simple_expr_prm(simple_expr_prm_type)) {

                if (simple_expr_prm_type == NO_T) {

                    simple_expr_type = term_type;
                } else if (term_type == simple_expr_prm_type) {

                    simple_expr_type = term_type;
                } else {

                    type_error(word);
                }

                // successfully parsed simple_expr
                return true;
            } else {

                // failed to parse simple_expr
                return false;
            }
        } else {

            // failed to parse term
            return false;
        }
    } else {

        // failed to find identifier, num, (, not, +, -
        string *expected = new string ("identifier, num, \"(\", not, \"+\" or \"-\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_expr_hat(expr_type &expr_hat_type) {

    expr_type simple_expr_type;

    // EXPR_HAT -> relop SIMPLE_EXPR
    //          -> LAMBDA
    // PREDICT(relop SIMPLE_EXPR) => {relop}

    // match relop
    if (word->get_token_type() == TOKEN_RELOP) {

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_simple_expr(simple_expr_type)) {

            if (simple_expr_type == INT_T) {

                expr_hat_type = INT_T;
            } else {

                type_error(word);
            }

            // successfully parsed expr_hat
            return true;
        }
    // PREDICT(EXPR_HAT -> LAMBDA) => {;, then, begin, , )}
    // match ; then begin , ) 
    } else if ((word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
            || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN)
            || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)
            || (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
            || (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)) {

        expr_hat_type = NO_T;

        // successfully parsed lambda
        return true;
    } else {

        // failed to find relop ; then begin , )
        string *expected = new string ("relop, \";\", then, begin, \",\" or \")\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_simple_expr_prm(expr_type &simple_expr_prm_type) {

    expr_type term_type, simple_expr_prm_type_1, addop_type;

    // SIMPLE_EXPR_PRM -> addop TERM SIMPLE_EXPR_PRM
    //                 -> LAMBDA
    // PREDICT(addop TERM SIMPLE_EXPR_PRM) => {addop}

    // match addop
    if (word->get_token_type() == TOKEN_ADDOP) {

        if ((static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
                || (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) {

            addop_type = INT_T;
        } else if (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_OR) {

            addop_type = BOOL_T;
        } else {

            type_error(word);
        }

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_term(term_type)) {

            if (parse_simple_expr_prm(simple_expr_prm_type_1)) {

                if (simple_expr_prm_type_1 == NO_T) {

                    if (addop_type == term_type) {

                        simple_expr_prm_type = addop_type;
                    } else {
                        type_error(word);
                    }
                } else if ((addop_type == term_type) && (term_type == simple_expr_prm_type_1)) {

                    simple_expr_prm_type = addop_type;
                } else {

                    type_error(word);
                }

                // successfully parsed simple_expr_prm
                return true;
            } else {

                // failed to parse simple_expr_prm
                return false;
            }
        } else {

            // failed to parse term
            return false;
        }

    // PREDICT(SIMPLE_EXPR_PRM -> LAMBDA) => {relop ; then begin , )}

    // match relop ; then begin , )
    } else if ((word->get_token_type() == TOKEN_RELOP)
            || (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
            || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN)
            || (word->get_token_type() == TOKEN_KEYWORD
                && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)
            || (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
            || (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)) {

        simple_expr_prm_type = NO_T;

        // successfully parsed lambda
        return true;
    } else {

        // failed to find relop ; then begin , )
        string *expected = new string ("relop, \";\", then, begin, \",\" or \")\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_term(expr_type &term_type) {

    expr_type factor_type, term_prm_type;

    // TERM -> FACTOR TERM_PRM
    // PREDICT(FACTOR TERM_PRM) => {identifier, num, (, +, -, not}

    // match identifier, num, (, +, -, not
    if ((word->get_token_type() == TOKEN_ID)
        || (word->get_token_type() == TOKEN_NUM)
        || (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
        || (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) {

        if (parse_factor(factor_type)) {

            if (parse_term_prm(term_prm_type)) {

                if (term_prm_type == NO_T) {

                    term_type = factor_type;
                } else if (factor_type == term_prm_type) {

                    term_type = factor_type;
                } else {

                    type_error(word);
                }

                // successfully parsed term
                return true;
            } else {

                // failed to parse term_prm
                return false;
            }
        } else {

            // failed to parse factor
            return false;
        }
    } else {

        // failed to find identifier, num, (, not, +, -
        string *expected = new string ("identifier, num, \"(\", not, \"+\" or \"-\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_term_prm(expr_type &term_prm_type) {

    expr_type term_prm_type_1, factor_type, mulop_type;

    // TERM_PRM -> mulop FACTOR TERM_PRM
    //          -> LAMBDA
    // PREDICT(mulop FACTOR TERM_PRM) => {mulop}

    // match mulop
    if (word->get_token_type() == TOKEN_MULOP) {

        if ((static_cast<MulopToken *>(word)->get_attribute() == MULOP_MUL)
            || (static_cast<MulopToken *>(word)->get_attribute() == MULOP_DIV)) {

            mulop_type = INT_T;
        } else if (static_cast<MulopToken *>(word)->get_attribute() == MULOP_AND) {

            mulop_type = BOOL_T;
        } else {

            type_error(word);
        }

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_factor(factor_type)) {

            if (parse_term_prm(term_prm_type_1)) {

                if ((term_prm_type_1 == NO_T) && (mulop_type == factor_type)) {

                    term_prm_type = mulop_type;
                } else if ((mulop_type == factor_type) && (factor_type == term_prm_type_1)) {

                    term_prm_type = mulop_type;
                } else {

                    type_error(word);
                }

                // successfully parsed term_prm
                return true;
            } else {

                // failed to parse term_prm
                return false;
            }
        } else {

            // failed to parse factor
            return false;
        }

    // PREDICT(TERM_PRM -> LAMBDA) => {addop relop ; then begin , )}

    // match addop relop ; then begin , )
    } else if ((word->get_token_type() == TOKEN_ADDOP)
        || (word->get_token_type() == TOKEN_RELOP)
        || (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
        || (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN)
        || (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)
        || (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
        || (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)) {

        term_prm_type = NO_T;

            // successfully parsed lambda
            return true;
    } else {

        // failed to find addop relop ; then begin , )
        string *expected = new string ("addop, relop, \";\", then, begin, \",\" or \")\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_factor(expr_type &factor_type) {

    expr_type the_expr_type, factor_type_1;

    // FACTOR -> identifier
    //        -> num
    //        -> ( EXPR )
    //        -> not FACTOR
    //        -> SIGN FACTOR

    // match identifier
    if (word->get_token_type() == TOKEN_ID) {

        if (stab->is_decl(static_cast<IdToken *>(word)->get_attribute(), current_env)) {

            factor_type = stab->get_type(static_cast<IdToken *>(word)->get_attribute(), current_env);
        } else {

            undeclared_id_error(static_cast<IdToken *>(word)->get_attribute(), current_env);
        }

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        // successfully parse factor
        return true;

    // match num
    } else if (word->get_token_type() == TOKEN_NUM) {

        factor_type = INT_T;

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        // successfully parsed num
        return true;

    // match (
    } else if (word->get_token_type() == TOKEN_PUNC 
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) {

        // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_expr(the_expr_type)) {

            if (word->get_token_type() == TOKEN_PUNC 
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

                factor_type = the_expr_type;

                // ADVANCE
                delete word;
                word = lex->next_token(); 

                // successfully parsed factor
                return true;
            } else {

                // failed to find )
                string *expected = new string ("\")\"");
                parse_error(expected, word);
                return false;
            }
        } else {

            // failed to parse expr
            return false;
        }
    

    // match not
    } else if (word->get_token_type() == TOKEN_KEYWORD
        && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT) {



        // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_factor(factor_type_1)) {

            if (factor_type_1 == BOOL_T) {

                factor_type = BOOL_T;
            } else {

                type_error(word);
            }

            return true;
        } else {

            return false;
        }

    // PREDICT(SIGN FACTOR) => {+ -}
    // match + -
    } else if ((word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
        || (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) {

            // ADVANCE
        delete word;
        word = lex->next_token(); 

        if (parse_factor(factor_type_1)) {

            if (factor_type_1 == INT_T) {

                factor_type = INT_T;
            } else {

                type_error(word);
            }

            return true;
        } else {

            return false;
        }
    } else {

        // failed to identifier, number (, not, +, -
        string *expected = new string ("identifier, number, \"(\", not, \"+\" or \"-\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}

bool Parser::parse_sign() {

    // SIGN -> +
    //      -> -

    // match +
    if (word->get_token_type() == TOKEN_ADDOP 
        && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        return true;
    // match -
    } else if (word->get_token_type() == TOKEN_ADDOP 
            && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        return true;
    } else {

        // failed to +, -
        string *expected = new string ("\"+\" or \"-\"");
        parse_error(expected, word);
        return false;
    }

    return false;
}
