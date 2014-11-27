/*
Author: John Foley
URL: thisfoley.ninja

Date: 9.25.14
Last Edit: 9.25.14
*/

#include "buffer.h"

/* Construct new buffer
 *  @args filename pointer to a series of a characters that make up the filename
 */
Buffer::Buffer ( char *filename ) {

	// Declare our source file stream and the buffer list container
	source_file = new std::ifstream ( filename , std::ifstream::in );
	b = new std::list<char>();
	fill_buf();
}


/* Deconstruct the object
 * Also, closes the buffer and deletes additional pointers
 */
Buffer::~Buffer () {

	// Does this delete everything?
	// Remember to match deletes and new's
	if ( source_file->is_open() ) {
		source_file->close();
	}
	delete source_file;
	delete b;
}

char Buffer::next_char() {

	if (b->empty()) {
		fill_buf();
	}

	char c = 0;
	c = b->front();
	b->pop_front();

	return c;
}

void Buffer::unread_char( char c ) {

	b->push_front(c);
}

void Buffer::buffer_fatal_error() {

	cout << "Oh, cruel fate. Why has thee foresaken our poor buffer" << endl;
	cout << "Buffer has encountered a fatal error. Exiting now" << endl;
	exit(0);
}

void Buffer::fill_buf() {

	int i = 0;
	char c = 0;
	bool done = false;

	while ( i < MAX_BUFFER_SIZE && !done) {		
	
		// Are we at the end of the file? push on a dollar sign
		if ( source_file->eof() ) {
			
			b->push_back('$');
			done = true;
		} else {

			// Pull in the next char
			c = source_file->get();

			// Make sure we didn't hit the end of the stream (or another error with the file stream)
			if  (source_file->good() ) {

				// Test to see if we pulled in a whitespace
				if ( is_whitespace(c) ) {
				
					// reduce to whitespace
					c = ' ';

					// push the space to the buffer
					b->push_back(c);
					i++;

					// get the next one
					c = source_file->get();

					// Consume whitespace
					while ( source_file->good() && is_whitespace(c) ) {

						// do nothing to consume more whitespace
						c = source_file->get();
					}

					// must make sure we didn't hit the end of the file (or other errors)
					if ( source_file->good() ) {

						// last char pulled wasn't whitespace, put back for next iteration
						source_file->putback(c);
					}

				} else if ( c == '#' ) {
				
					if (source_file->good()) {
							
						c = source_file->get();
						while (c != '\n') {
							
							if (source_file->good()) {
								c = source_file->get();
							}
						}
					}
				} else {

					// wasn't end of file or white space!
					b->push_back(c);

					i++;
				}
			}
		}
	}
}

void Buffer::dump_b() {

	list<char>::iterator it;
	for (it = b->begin(); it != b->end(); it++) {
		cout << *it ;
	}
	cout << endl;
}
