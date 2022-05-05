//***************************************************************************
//
//  Luke
//  z1885842
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************#

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <unistd.h>
#include <cassert>

#include "hex.h"
#include "rv32i_decode.h"

using namespace std;
/**
 * decode is the main way to determine the decoding process of this class.
 *
 * First, funct3 and funct7 is grabbed to help determine differents variations of some types.
 * 
 * After this is a ladder process of what kind of function is made, on default it gives an error.
 *
 * @param x addr, the memory address of the function.
 * 
 * @param y insn, the instruction at the memory address.
 *
 * @return The return is the string made by the decode process for printing.
 * 
 ********************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);

    switch(get_opcode(insn))
    {
        default:           return render_illegal_insn(insn);
        case opcode_lui:   return render_lui(insn);
        case opcode_auipc: return render_auipc(insn);
        case opcode_jal: return render_jal(addr, insn);
        case opcode_jalr: return render_jalr(insn);
        case opcode_load_imm:
            switch(funct3)
            {
                default:        return render_illegal_insn(insn);
                case funct3_lb: return render_itype_load(insn,"lb");
                case funct3_lh: return render_itype_load(insn,"lh");
                case funct3_lw: return render_itype_load(insn,"lw");
                case funct3_lbu: return render_itype_load(insn,"lbu");
                case funct3_lhu: return render_itype_load(insn,"lhu");
            } 
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
        case opcode_stype:
            switch(funct3)
            {
                default:        return render_illegal_insn(insn);
                case funct3_sb: return render_stype(insn,"sb");
                case funct3_sh: return render_stype(insn,"sh");
                case funct3_sw: return render_stype(insn,"sw");
            } 
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
        case opcode_rtype:
            switch(funct3)
            {
                default:           return render_illegal_insn(insn);
                case funct3_sll: return render_rtype(insn, "sll");  
                case funct3_slt: return render_rtype(insn, "slt");  
                case funct3_sltu: return render_rtype(insn, "sltu");  
                case funct3_xor: return render_rtype(insn, "xor");  
                case funct3_or: return render_rtype(insn, "or");    
                case funct3_and: return render_rtype(insn, "and"); 
                case funct3_add: 
                    switch(funct7)
                    {
                        default:         return render_illegal_insn(insn);
                        case funct7_add: return render_rtype(insn, "add");  
                        case funct7_sub: return render_rtype(insn, "sub"); 
                    }
                    assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
                case funct3_srx: 
                    switch(funct7)
                    { 
                        default:         return render_illegal_insn(insn);
                        case funct7_srl: return render_rtype(insn, "srl");  
                        case funct7_sra: return render_rtype(insn, "sra");
                    }
                    assert(0 && "unrecognized funct3"); //It should be impossible to reach this point         
            } 
                
        case opcode_system:
            switch(funct3)
            {
                case funct3_csrrw: return render_csrrx(insn, "csrrw");
                case funct3_csrrs: return render_csrrx(insn, "csrrs");
                case funct3_csrrc: return render_csrrx(insn, "csrrc");
                case funct3_csrrwi: return render_csrrxi(insn, "csrrwi");
                case funct3_csrrsi: return render_csrrxi(insn, "csrrsi");
                case funct3_csrrci: return render_csrrxi(insn, "csrrci");  
            }
            switch(insn)
            {
                default:           return render_illegal_insn(insn);
                case insn_ebreak: return render_ebreak(insn);
                case insn_ecall: return render_ecall(insn);
            }
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point      
                    
        case opcode_btype:
            switch(funct3)
            {
                default: return render_illegal_insn(insn);
                case funct3_beq: return render_btype(addr, insn, "beq");
                case funct3_bne: return render_btype(addr, insn, "bne");
                case funct3_blt: return render_btype(addr, insn, "blt");
                case funct3_bge: return render_btype(addr, insn, "bge");
                case funct3_bltu: return render_btype(addr, insn, "bltu");
                case funct3_bgeu: return render_btype(addr, insn, "bgeu");
            }
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
        case opcode_alu_imm:
            switch(funct3)
            {
                default: return render_illegal_insn(insn);
                case funct3_add: return render_itype_alu(insn, "addi", get_imm_i(insn));
                case funct3_slt: return render_itype_alu(insn, "slti", get_imm_i(insn));
                case funct3_sltu: return render_itype_alu(insn, "sltiu", get_imm_i(insn));
                case funct3_xor: return render_itype_alu(insn, "xori", get_imm_i(insn));
                case funct3_or: return render_itype_alu(insn, "ori", get_imm_i(insn));
                case funct3_and: return render_itype_alu(insn, "andi", get_imm_i(insn));
                case funct3_sll: return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
                case funct3_srx: 
                    switch(funct7)
                    {
                        default: return render_illegal_insn(insn);
                        case funct7_sra: return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
                        case funct7_srl: return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
                    }
                    assert(0 && "unrecognized funct7"); //It should be impossible to reach this point
            }
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point          
    }
    assert(0 && "unrecognized opcode"); //It should be impossible to reach this point
}
//helper functions
/**
 * get_opcode is the bitmask return for getting an opcode.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The opcode part of the insn is returned.
 * 
 ********************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
	return (insn & 0x0000007f);
}
/**
 * get_rd is the bitmask return for getting an rd.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The rd part of the insn is returned.
 * 
 ********************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    return ((insn & 0x00000f80) >> 7);
}
/**
 * get_funct3 is the bitmask return for getting an funct3.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The funct3 part of the insn is returned.
 * 
 ********************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    return ((insn & 0x00007000) >> 12);
}
/**
 * get_rs1 is the bitmask return for getting an rs1.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The rs1 part of the insn is returned.
 * 
 ********************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    return ((insn & 0x000f8000) >> 15);
}
/**
 * get_rs2 is the bitmask return for getting an rs2.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The rs2 part of the insn is returned.
 * 
 ********************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    return ((insn & 0x01f00000) >> 20); 
}
/**
 * get_funct7 is the bitmask return for getting an funct7.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The funct7 part of the insn is returned.
 * 
 ********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
    return ((insn & 0xfe000000) >> 25);
}
/**
 * get_imm_i is the bitmask return for getting an imm_i.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The imm_i part of the insn is returned.
 * 
 * @note Bit shifting is required, as well as a sign-extention for negative numbers.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
    int32_t imm_i ((insn & 0xfff00000) >> 20);
    if(insn & 0x80000000)
        imm_i |= 0xfffff000; //sign-extend the left;
    return imm_i;
}
/**
 * get_imm_u is the bitmask return for getting an imm_u.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The imm_u part of the insn is returned.
 * 
 ********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    return (insn & 0xfffff000);
}
/**
 * get_imm_b is the bitmask return for getting an imm_b.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The imm_b part of the insn is returned.
 * 
 * @note Bit shifting is required, as well as a sign-extention for negative numbers.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    //return ((insn & 0x80000000) >> 19) + ((insn & 0x00000080) << 4) + ((insn & 0x7e000000) >> 20) + ((insn & 0x00000f00) >> 7);
    //31 -> 12 (Down 19) >>
    //7-> 11 (Up 4) <<
    //30-25 -> 10-5 (Down 20) >>
    //11-8 -> 4-1 (Down 7)

    int32_t imm_s = ((insn & 0x80000000) >> 19);
    imm_s |= ((insn & 0x00000080) << 4);
    imm_s |= ((insn & 0x7e000000) >> 20);
    imm_s |= ((insn & 0x00000f00) >> 7);

    if(insn & 0x80000000)
        imm_s |= 0xfffff000; //sign-extend the left;

    return imm_s;
}
/**
 * get_imm_s is the bitmask return for getting an imm_s.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The imm_s part of the insn is returned.
 * 
 * @note Bit shifting is required, as well as a sign-extention for negative numbers.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
    //return ((insn & 0xfe000000) >> 20) + ((insn & 0x00000f80) >> 7);
    //31-25 -> 11-5 (Down 20)
    //11-7 -> 4-0 (Down 7)

    int32_t imm_s = ((insn & 0xfe000000) >> 20);
    imm_s |= ((insn & 0x00000f80) >> 7);

    if(insn & 0x80000000)
        imm_s |= 0xfffff000; //sign-extend the left;
    
    return imm_s;
}
/**
 * get_imm_j is the bitmask return for getting an imm_j.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The imm_j part of the insn is returned.
 * 
 * @note Bit shifting is required, as well as a sign-extention for negative numbers.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
    //return (insn & 0x000ff000) + ((insn & 0xfff00000) >> 20);
    //19-12 -> 19-12 (Normal)
    //31-20 -> 11-0 (Down 20)

    int32_t imm_s = (insn & 0x000ff000);
    imm_s |= ((insn & 0xfff00000) >> 20);

    if(insn & 0x80000000)
        imm_s |= 0xfffff000; //sign-extend the left;

    return imm_s;
}
/**
 * render_illegal_insn is the error message if an insn isn't regonized.
 *
 * @param x insn, insn is the instruction. 
 *
 * @return The error message is displayed.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    (void) insn;
    return " ERROR: UNIMPLEMENTED INSTRUCTION";
}
/**
 * render_reg is the formatting for registers.
 *
 * @param x r, is the int to be formatted. 
 *
 * @return The same number, with an x in front.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
    std::ostringstream os;
    os << "x" << r;
    return os.str();
}
/**
 * render_base_disp is the formatting for bases and displacements.
 *
 * @param x base, is the base to be formatted. 
 *
 * @param y disp, is the displacement to be formatted. 
 * 
 * @return The string outputted is "disp(base)"
 * 
 ********************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp)
{
    //uint32_t rd = get_rd(base);

    std::ostringstream os;
    os << disp  << "(" << render_reg(base) << ")";
    return os.str();
}
/**
 * render_mnemonic is the formatting for the mnemonics.
 *
 * @param x m, is the string to be formatted. 
 * 
 * @return The string outputted is but with a set length of mnemonic width filled
 * with spaces.
 ********************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &m)
{
    std::ostringstream os;
    os << " "  << m << std::setw(mnemonic_width - m.length()) << " ";
    return os.str();
}
/**
 * render_lui is the output function if a lui instruction is found.
 *
 * @param x insn is the instruction code. 
 * 
 * @return The lui instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;

    os << rv32i_decode::render_mnemonic("lui") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12)&0x0fffff);
    return os.str();
}
/**
 * render_auipc is the output function if a auipc instruction is found.
 *
 * @param x insn is the instruction code. 
 * 
 * @return The auipc instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;

    os << rv32i_decode::render_mnemonic("auipc") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12)&0x0fffff);
    return os.str();
}
/**
 * render_jal is the output function if a jal instruction is found.
 *
 * @param x addr is the address of the instruction code. 
 * 
 * @param y insn is the instruction code.
 * 
 * @return The jal instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);
    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << "," << hex::to_hex0x32(imm_j + addr);
    return os.str();
}
/**
 * render_jalr is the output function if a jalr instruction is found.
 * 
 * @param x insn is the instruction code.
 * 
 * @return The jalr instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
    return os.str();
}
/**
 * render_btype is the output function if a btype instruction is found.
 *
 * @param x addr is the address of the instruction code. 
 * 
 * @param y insn is the instruction code.
 * 
 * @param z mnemonic is the mnemonic of the instruction code.
 * 
 * @return The btype instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << hex::to_hex0x32(imm_b + addr);
    return os.str();
}
/**
 * render_itype_load is the output function if a itype_load instruction is found.
 * 
 * @param x insn is the instruction code.
 * 
 * @param y mnemonic is the mnemonic of the instruction code.
 * 
 * @return The itype_load instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp(rs1,imm_i);
    return os.str();
}
/**
 * render_stype is the output function if a stype instruction is found.
 * 
 * @param x insn is the instruction code.
 * 
 * @param y mnemonic is the mnemonic of the instruction code.
 * 
 * @return The stype instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_s = get_imm_s(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic)  << render_reg(rs2) << "," << render_base_disp(rs1, imm_s);;
    return os.str();
}
/**
 * render_itype_alu is the output function if a itype_alu instruction is found.
 *
 * @param x addr is the address of the instruction code. 
 * 
 * @param y insn is the instruction code.
 * 
 * @param z mnemonic is the mnemonic of the instruction code.
 * 
 * @return The itype_alu instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rd = get_rd(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << (imm_i);
    return os.str();
}
/**
 * render_rtype is the output function if a rtype instruction is found.
 * 
 * @param x insn is the instruction code.
 * 
 * @param y mnemonic is the mnemonic of the instruction code.
 * 
 * @return The rtype instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t rd = get_rd(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
    return os.str();
}
/**
 * render_ecall is the output function if a ecall instruction is found.
 *
 * @param x insn is the instruction code.
 * 
 * @return The ecall instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
    std::ostringstream os;
    os << " ecall";
    return os.str();
}
/**
 * render_ebreak is the output function if a ebreak instruction is found.
 *
 * @param x insn is the instruction code.
 * 
 * @return The ebreak instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    std::ostringstream os;
    os << " ebreak";
    return os.str();
}
/**
 * render_csrrx is the output function if a csrrx instruction is found.
 * 
 * @param x insn is the instruction code.
 * 
 * @param y mnemonic is the mnemonic of the instruction code.
 * 
 * @return The csrrx instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(imm_i & 0x00000fff) << "," << render_reg(rs1);
    return os.str();
}
/**
 * render_csrrxi is the output function if a csrrxi instruction is found.
 * 
 * @param x insn is the instruction code.
 * 
 * @param y mnemonic is the mnemonic of the instruction code.
 * 
 * @return The csrrxi instruction and it's function parameters are outputted.
 * 
 ********************************************************************************/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(imm_i & 0x00000fff) << "," << rs1;
    return os.str();
}


