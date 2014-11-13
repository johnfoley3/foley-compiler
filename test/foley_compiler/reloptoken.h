/*
Author: John Foley
URL: thisfoley.ninja
Template by: Dr. Bob Matthews

Date: 9.25.14
Last Edit: 9.25.14
*/

#ifndef RELOPTOKEN_H
#define RELOPTOKEN_H

using namespace std;

#include "token.h"
#include <string>

typedef enum relop_attr { RELOP_EQ  = 300, 
			    RELOP_NEQ 		= 301,
			    RELOP_GT		= 302,
			    RELOP_GE		= 303,
			    RELOP_LT		= 304,
			    RELOP_LE		= 305,
			    RELOP_NO_ATTR       = 399 } relop_attr_type;		 

// All token classes are subclasses of Token
class RelopToken : public Token
{
 public:
  // Construct an uninitialized relop token.
  RelopToken();

  ~RelopToken();

  // Construct a relop token with an attribute.
  RelopToken(relop_attr_type attr);

  // Return the attribute of this token
  relop_attr_type get_attribute() const;

  // Set the attribute of this token
  void set_attribute(relop_attr_type attr);

  string *to_string();

 private:

  // This tokens attribute
  relop_attr_type attribute;
};

#endif
