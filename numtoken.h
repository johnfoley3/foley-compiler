/*
Author: John Foley
URL: thisfoley.ninja

Date: 9.25.14
Last Edit: 9.25.14
*/

#ifndef NUMTOKEN_H
#define NUMTOKEN_H

#include "token.h"
#include <string>

using namespace std;

// All token classes are subclasses of Token
class NumToken : public Token
{
 public:
  // Construct an uninitialized num token.
  NumToken();

  ~NumToken();

  // Construct a num token with an attribute.
  NumToken(string *attr);

  // Return the attribute of this token
  string *get_attribute() const;

  // Set the attribute of this token
  void set_attribute(string *attr);

  string *to_string();

 private:

  // This tokens attribute
  string *attribute;
};

#endif
