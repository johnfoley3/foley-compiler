/* 
 * Author: John Foley
 * Date: 9.28.14
 * Last Edited: 9.28.14
 */

#include "scanner.h"

using namespace std;

int main( int argc , char *argv[] ) {
	
	char *filename = (char *) argv[1];

	Scanner *scan = new Scanner( filename );

	Token *someToken = scan->next_token();

	while (*someToken->to_string() != "$") {

		cout << *someToken->to_string() << endl;
		someToken = scan->next_token();
	}

	cout << *someToken->to_string() << endl;
	
	return 0;
}
