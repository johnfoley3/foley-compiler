/*
Author: John Foley
URL: thisfoley.ninja

Date: 9.25.14
Last Edit: 9.25.14
*/

#ifndef EOFTOKEN_H
#define EOFTOKEN_H

#include "token.h"
#include <string>

using namespace std;

/* EofToken is a derived class (subclass) of Token */
class EofToken : public Token
{
 public:

  EofToken() ;

  ~EofToken();

  string *get_attribute() const;

  string *to_string();

 private:
 // Storage for the attribute of the token.
  string *attribute;
};

#endif
