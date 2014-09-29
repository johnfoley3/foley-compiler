/*
Author: John Foley
URL: thisfoley.ninja

Date: 9.25.14
Last Edit: 9.25.14
*/

#include "numtoken.h"

/* Create a new token with no attributes. */
NumToken::NumToken () : Token()
{
  // Set the token type and init the attribute to undefined.
  set_token_type (TOKEN_NUM);
  attribute = new string("UNITIALIZED IDENTIFIER ATTRIBUTE");
}

/* Create a new token with an attribute. */
NumToken::NumToken (string *attr) : Token()
{
  // Set the token type and init the attribute.
  set_token_type (TOKEN_NUM);
  attribute = attr;
}

/* Nothing to do here. */
NumToken::~NumToken ()
{
}

/* Return the token's attribute. */
string *NumToken::get_attribute() const
{
  return attribute;
}

/* Set the tokens attribute. */
void NumToken::set_attribute(string *attr)
{
  attribute = attr;
}

/* Extract the token type and attribute, and return a string*
   containing the information.  Used for debugging only.
*/
string *NumToken::to_string()
{
	return attribute;
}

  
  
