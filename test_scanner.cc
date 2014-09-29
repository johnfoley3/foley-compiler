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
	scan->next_token();

	return 0;
}
