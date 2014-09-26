#ifndef BUFFER_H
#define BUFFER_H

#include <fstream>
#include <iostream>
#include <list>

// To get exit()
#include <stdlib.h>

/* This is the maximum number of characters from the source file that
   we will read into memory at once.
*/
#define MAX_BUFFER_SIZE 1024

/* We use '$' to mark the end of the source file in
   the buffer.
*/
#define EOF_MARKER '$'
  
// Characters that have a special meaning to the buffer.
#define COMMENT_MARKER '#'
#define SPACE ' '

using namespace std;

class Buffer
{
 public:

  // Open the source file and initialize the buffer.
  Buffer(char *filename);
  
  // Close the file and cleanup.
  ~Buffer();
  
  // Remove the next character from the buffer and
  // return it.
  char next_char();
  
  // Put a character back at the front of the buffer.
  void unread_char (char c);
  
  
 private:
  // Storage for the buffer
  list<char> *b;
  
  // Input stream for the source.
  ifstream *source_file;
  
  /* If you encounter an error from which you can't recover, print an
     error message and call this method.
   */
  void buffer_fatal_error();
  
  /* Fill the buffer from the file.  The buffer filling code is
     where all the action happens in this calss.  This function
     is responsible for filling the buffer until it is full with
     characters from the file.  Comments should be omitted,
     whitespace should be compressed to a single space character,
     and a '$' should be inserted at the end of the program to
     mark the end of the source file.
  */
  void fill_buf();
  
  // Is c a whitespace char?
  inline bool is_whitespace (char c)
  {
    return (c == SPACE || c == '\t' || c == '\n');
  }
  
  // For debugging: dump the contents of the buffer on the screen.
  void dump_b();
  
  
};

#endif
