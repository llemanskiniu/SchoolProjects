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
#include "memory.h"
#include "rv32i_decode.h"

#ifndef RV32I_HART_H
#define RV32I_HART_H

class rv32i_hart : public rv32i_decode
{
    public:
        rv32i_hart(memory &m) : mem(m) {};
        void set_show_instructions(bool b) {show_instructions = b; }
        void set_show_registers(bool b) {show_registers = b; }
        bool is_halted() const {return halt; }
        const std::string &get_halt_reason() const  { return halt_reason; } 
        uint64_t get_insn_count() const { return insn_counter; }       
        void set_mhartid(int i) { mhartid = i; }

        void tick(const std::string &hdr=""); //if hart is halted do nothing, else increment insn, show regiusters, fetch instruction, print hdr, call exec
        void dump (const std::string &hdr="") const; //dump the regs and the pc register (call the register member)
        void reset(); //reset the object, set PC to zero, call regs reset, insn counter, halt flag to false, halt reason to none

    private:
        void exec(uint32_t insn, std::ostream* pos); //executes given the rv32i instruction, using get_xxx() same switch logic as decode
        void exec_illegal_insn(uint32_t insn, std::ostream* pos); //set halt flah then void insn, render error message
        
        //void exec_xxx(uint32_t insn, std::ostream* pos); helper function (make multiple), simulates execution of instruction. (most complicated)

        
        void exec_lui(uint32_t insn, std::ostream* pos);
        void exec_auipc(uint32_t insn, std::ostream* pos);
        void exec_jal(uint32_t insn, std::ostream* pos);
        void exec_jalr(uint32_t insn, std::ostream* pos);
        void exec_lb(uint32_t insn, std::ostream* pos);
        void exec_lh(uint32_t insn, std::ostream* pos);
        void exec_lw(uint32_t insn, std::ostream* pos);
        void exec_lbu(uint32_t insn, std::ostream* pos);
        void exec_lhu(uint32_t insn, std::ostream* pos);
        void exec_sb(uint32_t insn, std::ostream* pos);
        void exec_sh(uint32_t insn, std::ostream* pos);
        void exec_sw(uint32_t insn, std::ostream* pos);
        void exec_sll(uint32_t insn, std::ostream* pos);
        void exec_slt(uint32_t insn, std::ostream* pos);
        void exec_sltu(uint32_t insn, std::ostream* pos);
        void exec_xor(uint32_t insn, std::ostream* pos);
        void exec_or(uint32_t insn, std::ostream* pos);
        void exec_and(uint32_t insn, std::ostream* pos);
        void exec_add(uint32_t insn, std::ostream* pos);
        void exec_sub(uint32_t insn, std::ostream* pos);
        void exec_srl(uint32_t insn, std::ostream* pos);
        void exec_sra(uint32_t insn, std::ostream* pos);
        void exec_csrrw(uint32_t insn, std::ostream* pos);
        void exec_csrrs(uint32_t insn, std::ostream* pos);
        void exec_csrrc(uint32_t insn, std::ostream* pos);
        void exec_csrrwi(uint32_t insn, std::ostream* pos);
        void exec_csrrsi(uint32_t insn, std::ostream* pos);
        void exec_csrrci(uint32_t insn, std::ostream* pos);
        void exec_ebreak(uint32_t insn, std::ostream* pos);
        void exec_ecall(uint32_t insn, std::ostream* pos);
        void exec_beq(uint32_t insn, std::ostream* pos);
        void exec_bne(uint32_t insn, std::ostream* pos);
        void exec_blt(uint32_t insn, std::ostream* pos);
        void exec_bge(uint32_t insn, std::ostream* pos);
        void exec_bltu(uint32_t insn, std::ostream* pos);
        void exec_bgeu(uint32_t insn, std::ostream* pos);
        void exec_addi(uint32_t insn, std::ostream* pos);
        void exec_slti(uint32_t insn, std::ostream* pos);
        void exec_sltiu(uint32_t insn, std::ostream* pos);
        void exec_xori(uint32_t insn, std::ostream* pos);
        void exec_ori(uint32_t insn, std::ostream* pos);
        void exec_andi(uint32_t insn, std::ostream* pos);
        void exec_slli(uint32_t insn, std::ostream* pos);
        void exec_srai(uint32_t insn, std::ostream* pos);
        void exec_srli(uint32_t insn, std::ostream* pos);

        

        uint32_t instruction_width = 35;
        bool halt = {false};
        std::string halt_reason = {"none"};
        uint32_t mhartid = {0};
        bool show_instructions = {false};
        bool show_registers = {false};
        uint64_t insn_counter = {0};
        uint32_t pc = {0};

    protected:
        registerfile regs;
        memory &mem;

    

};

#endif