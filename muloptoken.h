/*
Author: John Foley
URL: thisfoley.ninja
Template by: Dr. Bob Matthews

Date: 9.25.14
Last Edit: 9.25.14
*/

#ifndef MULOPTOKEN_H
#define MULOPTOKEN_H

#include "token.h"
#include <string>

using namespace std;

typedef enum mulop_attr {   MULOP_MUL 		= 501,
			    MULOP_DIV		= 502,
			    MULOP_AND		= 503,
			    MULOP_NO_ATTR       = 599 } mulop_attr_type;		 

// All token classes are subclasses of Token
class MulopToken : public Token
{
 public:
  // Construct an uninitialized mulop token.
  MulopToken();

  ~MulopToken();

  // Construct a mulop token with an attribute.
  MulopToken(mulop_attr_type attr);

  // Return the attribute of this token
  mulop_attr_type get_attribute() const;

  // Set the attribute of this token
  void set_attribute(mulop_attr_type attr);

  string *to_string();

 private:

  // This tokens attribute
  mulop_attr_type attribute;
};

#endif
