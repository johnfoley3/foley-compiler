/*
Author: John Foley
URL: thisfoley.ninja

Date: 9.25.14
Last Edit: 9.25.14
*/

#ifndef PUNCTOKEN_H
#define PUNCTOKEN_H

#include "token.h"
#include <string>

using namespace std;

typedef enum punc_attr { PUNC_SEMI  	= 200, 
			 PUNC_COLON 	= 201,
			 PUNC_COMMA 	= 202,
			 PUNC_ASSIGN 	= 203,
			 PUNC_OPEN	= 204,
			 PUNC_CLOSE  	= 205,
			 PUNC_NO_ATTR	= 299 } punc_attr_type;		 

// All token classes are subclasses of Token
class PuncToken : public Token
{
 public:
  // Construct an uninitialized punctuation token.
  PuncToken();

  ~PuncToken();

  // Construct a punctuation token with an attribute.
  PuncToken(punc_attr_type attr);

  // Return the attribute of this token
  punc_attr_type get_attribute() const;

  // Set the attribute of this token
  void set_attribute(punc_attr_type attr);

  string *to_string();

 private:

  // This tokens attribute
  punc_attr_type attribute;
};

#endif
