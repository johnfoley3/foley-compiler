#ifndef REGISTER_ALLOCATOR_H
#define REGISTER_ALLOCATOR_H

// The total number of registers
#define N_REGS 4
// R3 is reserved
#define STACK_REG 3

#include <iostream>
#include <stdlib.h>

// The register allocator manages Register objects.
#include "register.h"

// If we spill a register, we need emit code to move
// the register to memory.
#include "emitter.h"

// If we spill a register, we need to install a temporary
// memory location in the symbol_table.
#include "symbol_table.h"

using namespace std;

class Register_Allocator
{
public:
	Register_Allocator(Emitter *e, Symbol_Table *stab);
	~Register_Allocator();

	// Get a free register.
	Register *allocate_register();

	// Free a previously allocated register.
	void deallocate_register(Register *r);

private:
	// The register set we can allocate from.
	Register *register_set[N_REGS];

	// We need to access the emitter and symbol table if we have
	// to spill a register.
	Emitter *e;
	Symbol_Table *stab;

	// The number of the next register to spill, if necessary.
	int next_to_spill;
	
	// The compiler has its own, unique environment which 
        // can not conflict with any legal TruPL identifier.  
	// This is the environment used in the symbol table when 
	// we create temporary scratch memory locations to 
	// spill registers into.
	string *compiler_env;

	// Internal error routine.
	void freeing_unallocated_register(Register *reg);
};
#endif
