
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
#include <iomanip>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <unistd.h>
#include <sstream>

#include "memory.h"
#include "rv32i_decode.h"


#include "cpu_single_hart.h"
//#include "rv32i_hart.h"
//#include "registerfile.h"


using namespace std;
/**
 * usage only spits out an error message if the file cannot be found or has an error.
 *
 * @note This will immediatly end the program with an error output.
 ********************************************************************************/
static void usage()
{
	cerr << "Usage: rv321i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
	cerr << "    -d show dissassembly before program" << endl;
	cerr << "    -i show instruction printing during execution" << endl;
	cerr << "    -l maximum number of instructions to exec" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	cerr << "    -r show register printing during execution" << endl;
	cerr << "    -z show a dump of the regs & memory after simulation" << endl;
	exit(1);
}
/**
 * disassemble is the main function to use the decode class.
 *
 * First, a loop is made for every 4 bytes of the memory storage, and then this
 * memory is disassembled with the decode function with an output.
 *
 * @param x mem is the memory object created with main.
 * 
 ********************************************************************************/
static void disassemble(const memory &mem)
{
	//Enter Code Here
	for(uint32_t addr = 0; addr <  mem.get_size(); addr+=4)
	{
		cout << hex::to_hex32(addr);
		cout << ": ";
		cout << hex::to_hex32(mem.get32(addr));
		cout << " ";
		cout << rv32i_decode::decode(addr, mem.get32(addr));
		cout << endl;
	}
}
/**
 * main is the main usage of this program.  It controls all of functions.
 *
 * In assignment 5, first a memory limit is made for how big the memory object can be.
 * 
 * Several flags are made to ensure the program works as intended.
 *
 * The switch statement to ensure the proper command line arguments.
 *
 * Then, the file input is searched and checked if it's available, if not, the program ends.
 * 
 * Dissassembly happens here if it's checked.
 *
 * After this, the hart runs.
 * 
 * And finally, a memory dump of the hart and memory.
 *
 * @param x argc, argc is the command line inputs for contolling memory and other various commands.
 * 
 * @param y argv, argv is the file name input. 
 *
 * @return The main returns 0 if the program runs correctly.
 *
 * @note The assignment 3/4 code is still here for testing and functionality purposes.
 ********************************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100; //default memory size is 0x100

	memory mem(memory_limit);

	cpu_single_hart hart(mem);

	uint32_t exec_limit = 0;

	bool dump_after_sim = false;

	bool disassemble_before = false;

	int opt;
	while((opt = getopt(argc, argv, "dil:rzm:")) != -1)
	{
		switch(opt)
		{
			case 'd': //dissassembly of memory before program begins.
				{
					//load the file into memory before diasssembly
					disassemble_before = true;
					//disassemble(mem);
					//cpu_single_hart cpu(mem);
					//cpu.reset();
				}
				break;
			case 'i': //show instruction printing during execution
				{
					//some how make show_instructions(true)
					hart.set_show_instructions(true);
				}
				break;
			case 'l': //limit of instructions to execute (use the -m optarg to get a number)
				{
					//set exec_limit to the number (optarg)
					std::istringstream iss(optarg);
					iss >> std::hex >> exec_limit;
				}
				break;
			case 'r': //show a dump of the hart before each instruction
				{
					//dump hart before
					hart.set_show_registers(true);
				}
				break;
			case 'z': //show a dump of the hart after the simulation ends.
				{
					//dump hart after
					dump_after_sim = true;
				}
				break;
			case 'm': //size of simulated memory
				{

					std::istringstream iss(optarg);
					iss >> std::hex >> memory_limit;
				}
				break;
			default:
				usage();
		}
	}
	if(optind >= argc)
		usage(); //mising filename

	
	//mem.memory::dump();

	if(!mem.memory::load_file(argv[optind]))
		usage();

	if(disassemble_before)
	{
		disassemble(mem);
		cpu_single_hart cpu(mem);
		cpu.reset();
	}
	//mem.memory::dump();

	/*

	Assignment 3

	cout << mem.get_size() << endl;
	cout << hex::to_hex32(mem.get8(0)) << endl;
	cout << hex::to_hex32(mem.get16(0)) << endl;
	cout << hex::to_hex32(mem.get32(0)) << endl;
	cout << hex::to_hex0x32(mem.get8(0)) << endl;
	cout << hex::to_hex0x32(mem.get16(0)) << endl;
	cout << hex::to_hex0x32(mem.get32(0)) << endl;
	cout << hex::to_hex8(mem.get8(0)) << endl;
	cout << hex::to_hex8(mem.get16(0)) << endl;
	cout << hex::to_hex8(mem.get32(0)) << endl;
	cout << hex::to_hex0x32(mem.get32(0x1000)) << endl;
	
	
	mem.set8(0x10, 0x12);
	mem.set16(0x14, 0x1234);	
	mem.set32(0x18, 0x87654321);

	cout << hex::to_hex0x32(mem.get8_sx(0x0f)) << endl;
	cout << hex::to_hex0x32(mem.get8_sx(0x7f)) << endl;
	cout << hex::to_hex0x32(mem.get8_sx(0x80)) << endl;
	cout << hex::to_hex0x32(mem.get8_sx(0xe3)) << endl;
	cout << hex::to_hex0x32(mem.get16_sx(0xe0)) << endl;
	cout << hex::to_hex0x32(mem.get32_sx(0xe0)) << endl;
	*/
	
	//Assignment 4
	//disassemble(mem);
	//mem.memory::dump();

	//Assignment 5

	//std::cout << "\nRun\n";
	hart.run(exec_limit);

	if(dump_after_sim)
	{
		hart.dump();
		mem.dump();
	}

	return 0;
}
