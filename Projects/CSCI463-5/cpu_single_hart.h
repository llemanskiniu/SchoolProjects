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

#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H

#include "rv32i_hart.h"

class cpu_single_hart : public rv32i_hart
{
    public:
        cpu_single_hart(memory &mem) : rv32i_hart(mem){}; //constructor
        void run(uint64_t exec_limit); //set register 2 to memory size, note bytes in memory, call tick until is halted

    
};

#endif