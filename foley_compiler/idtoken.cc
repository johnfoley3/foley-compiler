/*
Author: John Foley
URL: thisfoley.ninja

Date: 9.25.14
Last Edit: 9.25.14
*/

#include "idtoken.h"

/* The constructor for the ID Token.  We call the base class constructor here.  
   This isn't strictly necessary, because the default constructor of the base
   class will be called anyway, but we do it here for completeness.
*/
IdToken::IdToken() : Token()
{
  set_token_type (TOKEN_ID);
  attribute = new string("UNINITIALIZED IDENTIFIER ATTRIBUTE");
}

IdToken::IdToken (string *attr) : Token()
{
  set_token_type (TOKEN_ID);
  attribute = new string(*attr);
}

IdToken::~IdToken()
{  if (attribute != NULL) {
    delete attribute;
  }
}


string *IdToken::get_attribute() const
{
  string *attr = new string(*attribute);
  return attr;
}


void IdToken::set_attribute(string *attr)
{ 
  if (attribute != NULL) {
    delete attribute;
  }
  attribute = new string (*attr);
}


string *IdToken::to_string()
{
  string *attribute_name = new string ("ID:" + *attribute);
  return attribute_name;
}
