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
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "hex.h"

using namespace std;
/**
 * To_hex8 returns a string of two hex digits from a unsigned 8 bit int.
 *
 * static_cast is needed to prevent the 8-bit integer being treated as a character.
 *
 * @param x uint8_t i The input 8-bit integer.
 *
 * @return The two digit hex string.
 *
 * @note Obtained from the teacher.
 ********************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
	ostringstream os; //os output for string
	os << std::hex << setfill('0') << setw(2) << static_cast<uint16_t>(i);
	return os.str();
}
/**
 * To_hex32 returns a string of eight hex digits from a unsigned 32 bit int.
 *
 * static_cast is needed to prevent the 32-bit integer being treated as a character.
 *
 * @param x uint32_t i The input 32-bit integer.
 *
 * @return The eight digit hex string.
 *
 * @note Modified from the original from the teacher.
 ********************************************************************************/
std::string hex::to_hex32(uint32_t i)
{
	ostringstream os; //os output for string
	os << std::hex << setfill('0') << setw(8) << static_cast<uint32_t>(i);
	return os.str();
}
/**
 * To_hex0x32 returns a string of eight hex digits and an '0x'
 * from a unsigned 32 bit int.
 *
 * @param x uint32_t i The input 32-bit integer.
 *
 * @return The eight digit hex string with an added 0x.
 *
 * @note Because the code is already written this just calls to_hex32
 * 		 and adds a 0x
 ********************************************************************************/
std::string hex::to_hex0x32(uint32_t i)
{
	return string("0x") + to_hex32(i);
}
//
//Assignment 4 Addons
//
/**
 * To_hex0x20 returns a string of five hex digits and an '0x'
 * from a unsigned 32 bit int.
 *
 * @param x uint32_t i The input 32-bit integer.
 *
 * @return The five digit hex string with an added 0x.
 *
 ********************************************************************************/
std::string hex::to_hex0x20(uint32_t i)
{
	ostringstream os; //os output for string
	os << std::hex << setfill('0') << setw(5) << static_cast<uint32_t>(i);
	return string("0x") +os.str();
}
/**
 * To_hex0x12 returns a string of three hex digits and an '0x'
 * from a unsigned 32 bit int.
 *
 * @param x uint32_t i The input 32-bit integer.
 *
 * @return The three digit hex string with an added 0x.
 *
 ********************************************************************************/
std::string hex::to_hex0x12(uint32_t i)
{
	ostringstream os; //os output for string
	os << std::hex << setfill('0') << setw(3) << static_cast<uint16_t>(i);
	return string("0x") +os.str();
}