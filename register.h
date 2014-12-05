#ifndef REGISTER_H
#define REGISTER_H

// To get STL stack
#include <stack>
#include <iostream>

#include <stdlib.h>

using namespace std;

// This class describes a single register.  You should not manipulate
// register objects directly except to get their number via get_num().
// 
// Instead, call the methods in the register allocator class to
// allocate and deallocate registers.
class Register
{
public:
	Register(int num);
	~Register();

	// What is this register's number?
	int get_num ();

	// Mark the register as inuse and unavailable for allocation
	void set_inuse();

	// Mark the register as free and available for allocation
	void clear_inuse();

	// Is the register allocated?
	bool is_inuse();

	// Crude register allocation scheme.
	void spill (string *location);
	string *unspill();
	bool has_spilled();
	

private:
	int num;
	bool inuse;
	
	// The stack used in the register allocation scheme.
	stack<std::string *> *spill_stack;

};
#endif
