/*
Author: John Foley
URL: thisfoley.ninja
Template by: Dr. Bob Matthews

Date: 9.25.14
Last Edit: 9.25.14
*/

/* This is a multiple include guard.  This prevents the programmer
   from inadvertently including a header more than once.  All of your
   header files should include an ifndef..define..endif include guard.
 */
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

/* An enum is like a series of int constant defintions,
   except the enum provides some rudimentary type checking.
*/
typedef enum token_type { TOKEN_KEYWORD =  0,
			  TOKEN_PUNC    =  1,
			  TOKEN_RELOP   =  2,
			  TOKEN_ADDOP   =  3,
			  TOKEN_MULOP   =  4,
			  TOKEN_ID      =  5,
			  TOKEN_NUM     =  6,
			  TOKEN_EOF     =  7,
			  TOKEN_NO_TYPE = 99 } token_type_type;

class Token
{

 public:
  // Default constructor
  Token();

  // Default destructor
  ~Token();

  /* Print the token type and it's attributes.  This method is for
     debugging purposes only.
 
     The funky notation in the declaration does two things: it tells
     the C++ compiler that the programmer must override the definition
     of to_string in the subclass (derived class in C++ speak), and it
     makes the function a "pure virtual function."  A PVF is like an
     abstract method in Java.  It prevents the user from creating any
     objects from the superclass (base class in C++.) */
  virtual string *to_string() = 0;

  // Set the token type.
  void set_token_type (token_type_type type);


  /* Return the type of this token.  The const keyword indicates to
     the compiler that the function will not alter any of the members
     of the object.  Basically, you are telling the compiler that the
     function is a pure accessor.
  */
  token_type_type get_token_type() const;

 private:
  // The type of the token
  token_type_type type;
  
};

#endif
