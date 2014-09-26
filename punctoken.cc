#include "punctoken.h"

/* Create a new token with no attributes. */
PuncToken::PuncToken () : Token()
{
  // Set the token type and init the attribute to undefined.
  set_token_type (TOKEN_PUNC);
  attribute = PUNC_NO_ATTR;
}


/* Create a new token with an attribute. */
PuncToken::PuncToken (punc_attr_type attr) : Token()
{
  // Set the token type and init the attribute.
  set_token_type (TOKEN_PUNC);
  attribute = attr;
}

/* Nothing to do here. */
PuncToken::~PuncToken ()
{
}

/* Return the token's attribute. */
punc_attr_type PuncToken::get_attribute() const
{
  return attribute;
}

/* Set the tokens attribute. */
void PuncToken::set_attribute(const punc_attr_type attr)
{
  attribute = attr;
}

/* Extract the token type and attribute, and return a string*
   containing the information.  Used for debugging only.
*/
string *PuncToken::to_string()
{
  string *attribute_name;

  switch (attribute) {
  case PUNC_SEMI:
    attribute_name = new string ("SEMICOLON");
    break;
  case PUNC_COLON:
    attribute_name = new string ("COLON");
    break;
  case PUNC_COMMA:
    attribute_name = new string ("COMMA");
    break;
  case PUNC_ASSIGN:
    attribute_name = new string ("ASSIGN");
    break;
  case PUNC_OPEN:
    attribute_name = new string ("OPEN");
    break;
  case PUNC_CLOSE:
    attribute_name = new string ("CLOSE");
    break;
  case PUNC_NO_ATTR:
    attribute_name = new string ("NO ATTRIBUTE");
    break;
  default:
    attribute_name = new string ("GARBAGE ATTRIBUTE VALUE");
    break;
  }

  string *result = new string("PUNCTUATION:" + *attribute_name);
  delete attribute_name;
  return result;
}

  
  
