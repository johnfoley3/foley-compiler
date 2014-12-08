#ifndef EMITTER_H
#define EMITTER_H

#include "register.h"

#include <iostream>

// Used in itos function
#include <sstream>

using namespace std;

// Instruction mnemonics
typedef enum instruction_type {MOVE = 802, 
			       ADD =  803,
			       SUB =  804, 
			       MUL =  805, 
			       DIV =  806, 
			       NEG =  807, 
			       NOT =  808, 
			       LEA =  809, 
			       BRUN = 810, 
			       BREZ = 811,
			       BRPO = 812, 
			       BRNE = 813, 
			       OUTB = 814,
			       HALT = 815, 
			       NOINST = 899} inst_type;
class Emitter {

public:
	
	Emitter();
	~Emitter();

	/* Label handling */

	// Generate a new, unique label
	string *get_new_label();

	// Generate a new, unique label with a specific
	// prefix.  Useful for debugging or self-
	// documenting the assembly language code.
	string *get_new_label(const char prefix[]);

	// Output a previously generated label
	void emit_label (string *label);


	/* Instruction handling */

	/* The first set handles the move instruction. */
	// For immediate mode
	void emit_move (Register *reg, int immedidate);
	// For register direct mode
	void emit_move (Register *reg, Register *regd);
	// For memory direct mode
	void emit_move (Register *reg, string *var);

	// Emit instructions of the form "move dest, Rn"
	//
	// Here is reg to memory move
	void emit_move (string *var, Register *reg);

	/* All the other two-address instructions are handled here.
	   The first address is always a register. */

	// To output "add R0, foovar", call
	// emit_2addr (ADD, <pointer to object for register 0>,
	//             <pointer to string containing "foovar">)
	void emit_2addr (inst_type inst,
			 Register *reg, int immediate);
	void emit_2addr (inst_type inst,
			 Register *reg, Register *src);
	void emit_2addr (inst_type inst,
			 Register *reg, string *var);

	/* One address instructions */

	void emit_1addr (inst_type inst, Register *reg);

	/* Branch instructions */

	// For brun
	void emit_branch (string *dest);
	// For the conditional branches with immmediate mode targets.
	void emit_branch (inst_type inst, 
			  Register *reg, int dest);
	// For conditional branches that target labels
	void emit_branch (inst_type inst,
			  Register *reg, string *dest);

	/* Halt instruction */
	void emit_halt();

	/* Data directives */
	void emit_data_directive (string *label, int size);
	void emit_data_directive (int size);

	/* If you want your compiler to add comments to your
	   Tral program, this is the ticket.  Hmm, compilers
	   that write comments!  */
	void emit_comment (const char comment[]);

private:
	// The current unique number used to generate each label.
	unsigned int label_num;

	// Convert an unsigned int to the corresponding
	// ASCII string
	string *itos (unsigned int i);
	
	// Emit an instruction mnemonic.
	void translate_and_emit (inst_type inst);

};
#endif
