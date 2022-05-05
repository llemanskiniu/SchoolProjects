//***************************************************************************
//
//  Luke
//  z1885842
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************



#include "cpu_single_hart.h"
/**
 * Run is the main loop of assignment 5.
 *
 * First, a divide is made if the exec limit is on.  If it isn't the program runs until a halt.  If it is, it runs either
 * until a halt or until the exec limit is reached.
 *
 * Once this loop ends, if the program is halted, it will give you the reason why.  The instruction count is also displayed.
 *
 * @param x exec_limit is the max amount of instructions the program executes. 
 *
 ********************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit) //set register 2 to memory size, note bytes in memory, call tick until is halted 
{

    //set register 2 to the memory size (with mem.get_size()
    //registerfile::set(1,mem.get_size());

    if(exec_limit == 0)
    {
        while(!(rv32i_hart::is_halted()))
        {
            //std::cout << "\nTick\n";
            rv32i_hart::tick();
        }
    }
    else
    {
        //int instruction_count = 0;
        while(  (!(rv32i_hart::is_halted())) && (exec_limit != rv32i_hart::get_insn_count())  )
        {
            //std::cout << "\nEXEC: " << exec_limit << " INSN: " << rv32i_hart::get_insn_count() << "\n";
            rv32i_hart::tick();
        }
    }
    if(rv32i_hart::is_halted())
    {
        std::cout << "Execution terminated. Reason: " << rv32i_hart::get_halt_reason() << "\n";
    }
    std::cout << rv32i_hart::get_insn_count() << " instructions executed\n";
}