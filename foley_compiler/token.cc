/*
Author: John Foley
URL: thisfoley.ninja
Template by: Dr. Bob Matthews

Date: 9.25.14
Last Edit: 9.25.14
*/

#include "token.h"

Token::Token()
{
  type = TOKEN_NO_TYPE;
}

Token::~Token()
{}

void Token::set_token_type (token_type_type type)
{
  this->type = type;
}

token_type_type Token::get_token_type() const
{
  return type;
}
