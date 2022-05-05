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



#include "registerfile.h"
#include "hex.h"

using namespace std;
/**
 * This is the main constructor for registers.
 *
 * The registers are filled with 32 objects, then reset is ran to fill them with their proper values.
 *
 ********************************************************************************/
registerfile::registerfile()
{
    for(int x = 0; x < 32; x++)
    {
        registers.push_back(0);
    }
    reset();
}
/**
 * Reset sets all of the registers to 0xf0f0f0f0 and the first to 0.
 *
 ********************************************************************************/
void registerfile::reset()
{
    registers[0] = 0;
    for (int x = 1; x < 32; x++)
    {
        registers[x] = 0xf0f0f0f0;
    }
}
/**
 * Set simply sets a value at r.
 *
 * @param x r is the position and register you set the value to.
 * 
 * @param y val is the value you insert.
 *
 ********************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
    registers[r] = val;
}
/**
 * get is a standard getter for the register r.
 *
 * @param x r is the register you are asking to return from.
 *
 * @return The main returns 0 if the program runs correctly.
 *
 ********************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
    return registers[r];
}
/**
 * dump shows all of the register values, in a neat fashion.
 *
 * @param x hdr is an optional header you can add to the dump.
 *
 ********************************************************************************/
void registerfile::dump(const std::string &hdr) const
{
    //std::cout << "\n";
    for(int x = 0; x < 4; x++)
    {
        cout << hdr << " ";
        if(x < 2)
            std::cout << " ";
        std::cout << "x" << x*8 << " ";
        for(int y = 0; y < 8; y++)
        {
            std::cout << hex::to_hex32(registers[(x*8 + y)]) << " ";
        }
        std::cout << "\n";
    }
}