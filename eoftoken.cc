/*
Author: John Foley
URL: thisfoley.ninja

Date: 9.25.14
Last Edit: 9.25.14
*/

#include "eoftoken.h"

/* The constructor for the ID Token.  We call the base class constructor here.  
   This isn't strictly necessary, because the default constructor of the base
   class will be called anyway, but we do it here for completeness.
*/
EofToken::EofToken() : Token()
{
  set_token_type (TOKEN_EOF);
  attribute = new string("$");
}

EofToken::~EofToken()
{  if (attribute != NULL) {
    delete attribute;
  }
}

string *EofToken::get_attribute() const {
	string *result = new string(*attribute);
	return result;
}

string *EofToken::to_string()
{
  return attribute;
}
