/*
Author: John Foley
URL: thisfoley.ninja
Template by: Dr. Bob Matthews

Date: 9.25.14
Last Edit: 9.25.14
*/

#include "muloptoken.h"

/* Create a new token with no attributes. */
MulopToken::MulopToken () : Token()
{
  // Set the token type and init the attribute to undefined.
  set_token_type (TOKEN_MULOP);
  attribute = MULOP_NO_ATTR;
}

/* Create a new token with an attribute. */
MulopToken::MulopToken (mulop_attr_type attr) : Token()
{
  // Set the token type and init the attribute.
  set_token_type (TOKEN_MULOP);
  attribute = attr;
}

/* Nothing to do here. */
MulopToken::~MulopToken ()
{
}

/* Return the token's attribute. */
mulop_attr_type MulopToken::get_attribute() const
{
  return attribute;
}

/* Set the tokens attribute. */
void MulopToken::set_attribute(const mulop_attr_type attr)
{
  attribute = attr;
}

/* Extract the token type and attribute, and return a string*
   containing the information.  Used for debugging only.
*/
string *MulopToken::to_string()
{
  string *attribute_name;

  switch (attribute) {
  case MULOP_MUL:
    attribute_name = new string ("MULTIPLY");
    break;
  case MULOP_DIV:
    attribute_name = new string ("DIVISION");
    break;
  case MULOP_AND:
    attribute_name = new string ("AND");
    break;
  case MULOP_NO_ATTR:
    attribute_name = new string ("NO ATTRIBUTE");
    break;
  default:
    attribute_name = new string ("GARBAGE ATTRIBUTE VALUE");
    break;
  }

  string *result = new string("MULOP:" + *attribute_name);
  delete attribute_name;
  return result;
}

  
  
