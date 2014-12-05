#include "register_allocator.h"

Register_Allocator::Register_Allocator(Emitter *e, Symbol_Table *stab)
{
  // Create the register set.
  for (int i = 0; i < N_REGS; i++) {
    register_set[i] = new Register(i);
  }

  // Reserve the stack register.  It can not be used except to
  // access the procedure call stack (which we will not be
  // doing.)
  register_set[STACK_REG]->set_inuse();

  this->e = e;
  this->stab = stab;

  // We will spill register 0 first, if necessary.
  next_to_spill = 0;

  // The compiler's unique environment in the symbol table if
  // we have to install temporary scratch memory locations.
  compiler_env = e->get_new_label ("_trupl_compiler");
}

Register_Allocator::~Register_Allocator()
{
  for (int i = 0; i < N_REGS; i++) {
    delete register_set[i];
  }
}

// Find a free register and return it.
Register *Register_Allocator::allocate_register()
{
  // Iterate over all the registers, looking for one that has
  // not been allocated.
  for (int i = 0; i < N_REGS; i++) {
    if (!register_set[i]->is_inuse()) {
      register_set[i]->set_inuse();
      return register_set[i];
    }
  }
	
  // All registers in use.  We need to spill.
  int spillee = next_to_spill;

  // We spill in this order, R0, R1, R2, R0, ...
  next_to_spill = (next_to_spill + 1) % (N_REGS - 1);

  // Create a label for the memory location where we will spill.
  string *location = e->get_new_label ("spill");
  // Add the new memory location to the symbol table.
  stab->install (location, compiler_env, INT_T);
  // Spill the register.
  register_set[spillee]->spill (location);
  e->emit_move (location, register_set[spillee]);

  return register_set[spillee];
}


// Make a previously allocated register available for reallocation.
void Register_Allocator::deallocate_register (Register *reg)
{
  if (!register_set[reg->get_num()]->is_inuse()) {
    // Ooops, your code generator is broken.
    freeing_unallocated_register(reg);
  }

  // We have to deallocate spilled and unspilled registers
  // differently.  Unspilled registers are simply marked as
  // available.  Spilled registers have to have their contents
  // at the last spill reloaded.
  if (reg->has_spilled()) {
    // This register has spilled.  Retrieve the value from memory.
    string *location = reg->unspill();
    e->emit_move (reg, location);
  } else {
    // Register hasn't spilled.  Just mark it free.
    register_set[reg->get_num()]->clear_inuse();
  }
}


void Register_Allocator::freeing_unallocated_register (Register *reg)
{
  cout << "Attempt to free unallocated register number ";
  cout << reg->get_num() << endl;
}
