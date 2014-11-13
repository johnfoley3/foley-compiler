/*
Author: John Foley
URL: thisfoley.ninja
Template by: Dr. Bob Matthews

Date: 9.25.14
Last Edit: 9.25.14
*/

#ifndef KEYWORDTOKEN_H
#define KEYWORDTOKEN_H

#include "token.h"
#include <string>

using namespace std;

typedef enum keyword_attr { KW_PROGRAM  = 100, 
			    KW_PROCEDURE = 101,
			    KW_INT      = 102, 
			    KW_BOOL     = 103,
			    KW_BEGIN    = 104, 
			    KW_END      = 105,
			    KW_IF       = 106, 
			    KW_THEN     = 107,
			    KW_ELSE     = 108, 
			    KW_WHILE    = 109,
			    KW_PRINT    = 110, 
			    KW_NOT      = 111,
			    KW_NO_ATTR  = 199 } keyword_attr_type;		 
// All token classes are subclasses of Token
class KeywordToken : public Token
{
 public:
  // Construct an uninitialized keyword token.
  KeywordToken();

  ~KeywordToken();

  // Construct a keywork token with an attribute.
  KeywordToken(keyword_attr_type attr);

  // Return the attribute of this token
  keyword_attr_type get_attribute() const;

  // Set the attribute of this token
  void set_attribute(keyword_attr_type attr);

  string *to_string();

 private:

  // This tokens attribute
  keyword_attr_type attribute;
};

#endif
