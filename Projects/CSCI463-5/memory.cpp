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
//#include <memory.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <unistd.h>

//#include "hex.cpp"
//#include "memory.h"

//#include "hex.h"
#include "memory.h"
//#include "hex.cpp"

using namespace std;
/**
 * Memory is the constructor of the memory class.
 *
 * @param x uint32_t siz An input for the size of the vector
 *
 * @note Given from the teacher
 ********************************************************************************/
memory::memory(uint32_t siz)
{
	siz = (siz+15)&0xfffffff0; //initial memory size, round the legnth up, mod-16
	mem.resize(siz, 0xa5);
}
/**
 * ~Memory is the destructor of the memory class
 *
 * Vectors come with a destructor.
 ********************************************************************************/
memory::~memory()
{
	mem.clear(); //clear the vector
}
/**
 * check_illegal checks if the address given, goes out of bounds.
 *
 * This function will compare if the address is larger or equal to the size.
 * If it is, it will give an error and return false, otherwise, it'll return true.
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @return The boolean value of if the address is illegal.
 *
 * @note The warning does not end the program, but a false value will stop
 * 		  many program functions from continuing
 ********************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
	if(!(i < memory::get_size())) //If i can't fit in the size...
			cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << endl; //out of bounds warning
	return (i > memory::get_size()); //Return result
}
/**
 * get_size returns the size of the vector's memory.
 *
 * @return The size of the vector, in unsigned 32-bit.
 *
 ********************************************************************************/
uint32_t memory::get_size() const
{
	return memory::mem.size(); //Return size of vector mem
}
/**
 * get8 will return the value in the address of the vector.
 *
 * If the address is out of bounds, checked by check_illegal, the function
 * will return 0 instead.
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @return The 8 bit unsigned value in the vector.
 *
 * @note This function is used in the other get functions as a shortcut.
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
	if(!(check_illegal(addr))) //if the addr is legal...
		return mem[addr]; //return the result
	else 
		return 0;
}
/**
 * get16 will return the 16 bit value in the address of the vector.
 *
 * This function uses two get8's, meaning it'll still be checked and
 * will return 0 if the address doesn't work.
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @return The 16 bit unsigned value in the vector.
 *
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
	uint16_t x = get8(addr) + ( get8(addr+1) << 8); //get two different addresses, next to each other.
	return x; //return the combination.
}
/**
 * get32 will return the 32 bit value in the address of the vector.
 *
 * This function uses two get16's, meaning it'll still be checked and
 * will return 0 if the address doesn't work.
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @return The 32 bit unsigned value in the vector.
 *
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
	uint32_t x = get16(addr) + (get16(addr+2) << 16); //get two different addresses, next to each other.
	return x; //return the combination.
}
/**
 * get8_sx will return a 32 bit address instead of an 8 bit one.
 *
 * This function only needs to use a get8, 
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @return The 32 bit unsigned value in the vector.
 *
 ********************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
	return (get8(addr)); //return the 32 bit addr.
}
/**
 * get16_sx will return a 32 bit address with two get8's
 *
 * This function uses two get8's to get a 16 bit address. 
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @return The 32 bit unsigned value in the vector.
 *
 ********************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const
{
	return ((get8(addr) + (get8(addr+1) << 8)) << 16); //return the 16 bit addr.
}
/**
 * get32_sx will return a 32 bit address with only a get32
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @return The 32 bit unsigned value in the vector.
 *
 ********************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const
{
	return get32(addr); //return the 32 byte addr.
}
/**
 * set8 will set a value in the vector at addr.
 *
 * First, an illegal check is made, and if the address is legal, the value
 * is inserted at the address. 
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @param y uint8_t val The value to be inserted.
 * 
 * @note This function will be used by the other set functions.
 ********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
	if(!(check_illegal(addr))) //if the addr is legal
		mem.at(addr) = val; //set the value to the addr.
	else
		return;
}
/**
 * set16 will set two values in the vector at the start of addr.
 *
 * Because the address is the starting point, the address must be pushed 1
 * and the value must be bit shifted 8.
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @param y uint8_t val The value to be inserted.
 ********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
	set8(addr, val); //Use set8 to set this part of the addr.
	set8(addr+1, val >> 8); //Then go one step ahead.
}
/**
 * set32 will set four values in the vector at the start of addr.
 *
 * Because the address is the starting point, the address must be pushed 2
 * and the value must be bit shifted 16.
 *
 * @param x uint32_t i The input 32-bit address.
 *
 * @param y uint8_t val The value to be inserted.
 ********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
	set16(addr, val); //Use set16 to set this part of the addr.
	set16(addr+2, val >> 16); //Then go one step ahead.
}
/**
 * dump is the output method to show the simulated memory of the vector.
 *
 * First, there must be a loop for every two bytes of memory, the address
 * of the memory must be shown, then the hex values using to_hex.
 * Finally, the ASCII values must be shown using  isprint.
 *
 ********************************************************************************/
void memory::dump() const
{
	uint32_t size = memory::get_size(); //size equals get size
	for(uint32_t x = 0; x < size/16; x++) //loop x < size/16 times for proper loop function
	{
		cout << hex::to_hex32(x*16) << ": "; //show which part of data the values are in
		for(int y = 0; y < 16; y++) //loop 16 times
		{
			cout <<  hex::to_hex8(get8((x*16)+y)) << " "; //get the 8 bit hex value of each part of the vector
			if(y == 7)
				cout << " "; //half way, place a space
		}
		cout << "*"; 
		uint8_t ch;
		for(int z = 0; z < 16; z++)
		{
			ch = get8((x*16) + z);
			ch = isprint(ch) ? ch : '.'; //show the actual ascii value in the vector, otherwise, put a "."
			cout << ch;
		}
		cout << "*" << endl;
	}	
}
/**
 * load_file, will open and read a file's binary contents.  If it can't be opened
 * it will return false.
 *
 * An error message is shown if the file cannot be opened.  The method here also
 * writes data to the simulated memory vector.
 *
 * @param x const std::string &fname The file name's pointer.
 * 
 * @return The boolean if the file loaded.
 ********************************************************************************/
bool memory::load_file(const std::string &fname)
{
	std::ifstream infile(fname, std::ios::in|std::ios::binary); //load the file

	if(!(infile.is_open())) //if it isn't open, close and give an error.
	{
		std::cerr << "Can't open file " << fname << " for reading." << endl;
		return false;
	}

	uint8_t i;
	infile >> std::noskipws;
	for (uint32_t addr = 0; infile >> i; ++addr) //read the file until it's empty
	{
		if(!(check_illegal(addr))) //if the addr is legal
		{
			set8(addr, i); //set the value into the vector
		}
		//Assignment 3 Correction: Checks for size if too big and sends false.
		else
			return false;
	}
	return true;
}
