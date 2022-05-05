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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <unistd.h>
#include <vector>

//#include "hex.h"

#ifndef REGISTERFILE_H
#define REGISTERFILE_H

class registerfile
{
    public:
        registerfile();
        void reset();
        void set(uint32_t r, int32_t val);
        int32_t get(uint32_t r) const;
        void dump(const std::string &hdr) const;

    private:
        std::vector<int32_t> registers;

    

};

#endif