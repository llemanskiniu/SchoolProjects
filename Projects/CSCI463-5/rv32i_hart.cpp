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

#include <cassert>

#include "rv32i_hart.h"
#include "rv32i_decode.h"
/**
 * Tick is the main function for this program.
 *
 * If performs in order:
 * First, the insn is gathered
 * The insn counter is incremented.
 * If the show registers flag is set, it will dump the registers.
 * If the instructions flag is set. It shows a more detailed process of what is happening.
 * Else, it will just execute without any instructions shown.
 *
 * @param x hdr is the optional header you can give each instruction.
 *
 ********************************************************************************/
void rv32i_hart::tick(const std::string &hdr) //if hart is halted do nothing, else increment insn, show regiusters, fetch instruction, print hdr, call exec
{
    if(!is_halted())
    {
        int32_t insn = mem.get32(pc);

        insn_counter++;

        if( show_registers )
        {
            dump(hdr);
        }
        //Fetch an instruction from the memory at the address in the PC register.
        if( show_instructions )
        {
            //std::cout << "\n";
            std::cout << hdr << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " ";
            //print the hdr, the pc register and the fetched instruction.
            exec(insn, &std::cout);
            std::cout << "\n";
        }
        else
        {
            exec(insn, nullptr);
            //std::cout << "\n";
        }
    }
}
/**
 * Dump uses the old dump function in register, with an added PC register.
 *
 * @param x hdr is the optional header you can give each dumped data.
 *
 ********************************************************************************/
void rv32i_hart::dump(const std::string &hdr) const //dump the regs and the pc register (call the register member)
{
    regs.dump(hdr);
    std::cout << "  pc " << hex::to_hex32(pc) << "\n";
}
/**
 * Reset simply resets the hart back to the original.
 *
 * The pc key is set to zero, the registers are reset, the insn counter is set to zero, halt is turned off and the reason set to none.
 *
 ********************************************************************************/
void rv32i_hart::reset() //reset the object, set PC to zero, call regs reset, insn counter, halt flag to false, halt reason to none
{
    pc = 0;
    regs.reset();
    insn_counter = 0;
    halt = false;
    halt_reason = "none";
}
/**
 * Exec is the large switch statement to try and find each executable function.  If an illegal or unfound instruction is found, an illegal insn will be thrown.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, each instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec(uint32_t insn, std::ostream* pos) //executes given the rv32i instruction, using get_xxx() same switch logic as decode
{
    //std::cout << "\nExec\n";
    uint32_t funct3 = rv32i_decode::get_funct3(insn);
    uint32_t funct7 = rv32i_decode::get_funct7(insn);

    switch(rv32i_decode::get_opcode(insn))
    {
        default:                    exec_illegal_insn(insn, pos); return;
        case opcode_lui:            exec_lui(insn, pos); return;
        case opcode_auipc:          exec_auipc(insn, pos); return;
        case opcode_jal:            exec_jal(insn, pos); return;
        case opcode_jalr:           exec_jalr(insn, pos); return;
        case opcode_load_imm:
            switch(funct3)
            {
                default:            exec_illegal_insn(insn, pos); return;
                case funct3_lb:     exec_lb(insn, pos); return;
                case funct3_lh:     exec_lh(insn, pos); return;
                case funct3_lw:     exec_lw(insn, pos); return;
                case funct3_lbu:    exec_lbu(insn, pos); return;
                case funct3_lhu:    exec_lhu(insn, pos); return;
            } 
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
        case opcode_stype:
            switch(funct3)
            {
                default:            exec_illegal_insn(insn, pos); return;
                case funct3_sb:     exec_sb(insn, pos); return;
                case funct3_sh:     exec_sh(insn, pos); return;
                case funct3_sw:     exec_sw(insn, pos); return;
            } 
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
        case opcode_rtype:
            switch(funct3)
            {
                default:            exec_illegal_insn(insn, pos); return;
                case funct3_sll:    exec_sll(insn, pos); return; 
                case funct3_slt:    exec_slt(insn, pos); return; 
                case funct3_sltu:   exec_sltu(insn, pos); return; 
                case funct3_xor:    exec_xor(insn, pos); return;
                case funct3_or:     exec_or(insn, pos); return;
                case funct3_and:    exec_and(insn, pos); return;
                case funct3_add: 
                    switch(funct7)
                    {
                        default:         exec_illegal_insn(insn, pos); return;
                        case funct7_add: exec_add(insn, pos); return; 
                        case funct7_sub: exec_sub(insn, pos); return;
                    }
                    assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
                case funct3_srx: 
                    switch(funct7)
                    { 
                        default:            exec_illegal_insn(insn, pos); return;
                        case funct7_srl:    exec_srl(insn, pos); return;  
                        case funct7_sra:    exec_sra(insn, pos); return;
                    }
                    assert(0 && "unrecognized funct3"); //It should be impossible to reach this point         
            } 
                
        case opcode_system:
            switch(funct3)
            {
                case funct3_csrrw:      exec_csrrw(insn, pos); return;
                case funct3_csrrs:      exec_csrrs(insn, pos); return;
                case funct3_csrrc:      exec_csrrc(insn, pos); return;
                case funct3_csrrwi:     exec_csrrwi(insn, pos); return;
                case funct3_csrrsi:     exec_csrrsi(insn, pos); return;
                case funct3_csrrci:     exec_csrrci(insn, pos); return; 
            }
            switch(insn)
            {
                default:                exec_illegal_insn(insn, pos); return;
                case insn_ebreak:       exec_ebreak(insn, pos); return;
                case insn_ecall:        exec_ecall(insn, pos); return;
            }
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point      
                    
        case opcode_btype:
            switch(funct3)
            {
                default:                exec_illegal_insn(insn, pos); return;
                case funct3_beq:        exec_beq(insn, pos); return;
                case funct3_bne:        exec_bne(insn, pos); return;
                case funct3_blt:        exec_blt(insn, pos); return;
                case funct3_bge:        exec_bge(insn, pos); return;
                case funct3_bltu:       exec_bltu(insn, pos); return;
                case funct3_bgeu:       exec_bgeu(insn, pos); return;
            }
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point
        case opcode_alu_imm:
            switch(funct3)
            {
                default:                exec_illegal_insn(insn, pos); return;
                case funct3_add:        exec_addi(insn, pos); return;
                case funct3_slt:        exec_slti(insn, pos); return;
                case funct3_sltu:       exec_sltiu(insn, pos); return;
                case funct3_xor:        exec_xori(insn, pos); return;
                case funct3_or:         exec_ori(insn, pos); return;
                case funct3_and:        exec_andi(insn, pos); return;
                case funct3_sll:        exec_slli(insn, pos); return;
                case funct3_srx: 
                    switch(funct7)
                    {
                        default:            exec_illegal_insn(insn, pos); return;
                        case funct7_sra:    exec_srai(insn, pos); return;
                        case funct7_srl:    exec_srli(insn, pos); return;
                    }
                    assert(0 && "unrecognized funct7"); //It should be impossible to reach this point
            }
            assert(0 && "unrecognized funct3"); //It should be impossible to reach this point          
    }
    assert(0 && "unrecognized opcode"); //It should be impossible to reach this point

        //need finishing
}
/**
 * exec_illegal_insn is the default instruction, which will give an error and halt the program.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos) //set halt flah then void insn, render error message
{
    (void)insn;
    if(pos)
        *pos << rv32i_decode::render_illegal_insn(insn);
    halt = true;
    halt_reason = "Illegal Instruction";
}

/**
 * exec_lui is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/

void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t imm_u = rv32i_decode::get_imm_u(insn);
    if(pos)
    {
        std::string s = rv32i_decode::render_lui(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(imm_u); 
    }
    regs.set(rd, imm_u);
    pc+=4;  
}
/**
 * exec_auipc is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t imm_u = rv32i_decode::get_imm_u(insn);
    if(pos)
    {
        std::string s = rv32i_decode::render_auipc(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_u) << " = " << hex::to_hex0x32(pc + imm_u); 
    }
    regs.set(rd, pc+imm_u);
    pc+=4;  
}
/**
 * exec_jal is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t imm_j = rv32i_decode::get_imm_j(insn);

    if(pos)
    {
        std::string s = rv32i_decode::render_jal(pc,insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(pc+4) << ", pc = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(imm_j+pc); 
    }
    regs.set(rd, pc+4);
    pc+=imm_j;  
}
/**
 * exec_jalr is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t imm_i = rv32i_decode::get_imm_i(insn);
    int32_t val = ((regs.get(rs1))+imm_i)&~1;

    if(pos)
    {
        std::string s = rv32i_decode::render_jalr(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(pc+4) << ", pc = (" << hex::to_hex0x32(imm_i) << " + " << hex::to_hex0x32(regs.get(rs1)) << ") & ";
        *pos << hex::to_hex0x32(~1) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, pc+4);
    pc = val; 
}
/**
 * exec_lb is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t imm_i = rv32i_decode::get_imm_i(insn);
    int32_t val = regs.get(rs1) + imm_i;
    val = mem.get8(val);
    val = val & 0x000000ff;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_load(insn, "lb    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_lh is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t imm_i = rv32i_decode::get_imm_i(insn);
    int32_t val = regs.get(rs1) + imm_i;
    val = mem.get16(val);
    val = val & 0x0000ffff;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_load(insn, "lh    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_lw is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t imm_i = rv32i_decode::get_imm_i(insn);
    int32_t val = regs.get(rs1) + imm_i;
    val = mem.get32(val);
    val = val & 0xffffffff;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_load(insn, "lw    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_lbu is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t imm_i = rv32i_decode::get_imm_i(insn);
    int32_t val = regs.get(rs1) + imm_i;
    val = mem.get8(val);
    val = val & 0x000000ff;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_load(insn, "lbu   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_lhu is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t imm_i = rv32i_decode::get_imm_i(insn);
    int32_t val = regs.get(rs1) + imm_i;
    val = mem.get16(val);
    val = val & 0x0000ffff;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_load(insn, "lhu   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_sb is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos)
{
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t imm_s = rv32i_decode::get_imm_s(insn);
    mem.set8(regs.get(rs1)+imm_s, regs.get(rs2) & 0x000000ff);
    if(pos)
    {
        std::string s = rv32i_decode::render_stype(insn, "sb    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(regs.get(rs2) & 0x000000ff);
    }
    pc+=4;  
}
/**
 * exec_sh is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos)
{
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t imm_s = rv32i_decode::get_imm_s(insn);
    mem.set16(regs.get(rs1)+imm_s, regs.get(rs2) & 0x0000ffff);
    if(pos)
    {
        std::string s = rv32i_decode::render_stype(insn, "sh    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// m16(" << hex::to_hex0x32(rs1) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(regs.get(rs2) & 0x0000ffff);
    }
    pc+=4;  
}
/**
 * exec_sw is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos)
{
    int32_t rs1 = rv32i_decode::get_rs1(insn);
    int32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t imm_s = rv32i_decode::get_imm_s(insn);
    mem.set32(regs.get(rs1)+imm_s, regs.get(rs2) & 0xffffffff);
    if(pos)
    {
        std::string s = rv32i_decode::render_stype(insn, "sw    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// m32(" << hex::to_hex0x32(rs1) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(regs.get(rs2) & 0xffffffff);
    }
    pc+=4;  
}
/**
 * exec_sll is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) << (regs.get(rs2))%XLEN);
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "sll   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " << rs2%XLEN << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_slt is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)
{
    
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1:0;

    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "slt   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val); 
    }
    regs.set(rd,val);
    pc+=4;    
}
/**
 * exec_sltu is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = ((uint32_t)regs.get(rs1)) < ((uint32_t)regs.get(rs2)) ? 1:0;
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "sltu  ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val); 
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_xor is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) ^ regs.get(rs2));
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "xor   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_or is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) | regs.get(rs2));
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "or    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_and is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) & regs.get(rs2));
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "and   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_add is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) + regs.get(rs2));
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "add   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_sub is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) - regs.get(rs2));
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "sub   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_srl is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) >> (regs.get(rs2)%XLEN));
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "srl   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << regs.get(rs2)%XLEN << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_sra is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    int32_t val = (regs.get(rs1) >> (regs.get(rs2)%XLEN));
    if(pos)
    {
        std::string s = rv32i_decode::render_rtype(insn, "sra   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << regs.get(rs2)%XLEN << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd,val);
    pc+=4;  
}
/**
 * exec_csrrw is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_csrrw(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
        std::string s = rv32i_decode::render_csrrx(insn, "csrrw ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    }
    pc+=4;  
}
/**
 * exec_csrrs is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)
{
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);

    uint32_t rd = rv32i_decode::get_rd(insn);
    if(pos)
    {
        std::string s = rv32i_decode::render_csrrx(insn, "csrrs ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << mhartid;
    }

    if((imm_i & 0x00000fff) == 0xf14)
    {
        regs.set(rd, mhartid );
        pc+=4;
    }
    else
    {
        halt = true;
        halt_reason = "Illegal CSR in CRRSS Instruction";
    }  
}
/**
 * exec_ csrrc is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_csrrc(uint32_t insn, std::ostream* pos)
{
    
    if(pos)
    {
        std::string s = rv32i_decode::render_csrrx(insn, "csrrc ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        
    }
    
    pc+=4;  
}
/**
 * exec_csrrwi is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_csrrwi(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
        std::string s = rv32i_decode::render_csrrxi(insn, "csrrwi ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    }
    pc+=4;  
}
/**
 * exec_csrrsi is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_csrrsi(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
        std::string s = rv32i_decode::render_csrrxi(insn, "csrrsi ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    }
    pc+=4;  
}
/**
 * exec_csrrsi is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_csrrci(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
        std::string s = rv32i_decode::render_csrrxi(insn, "csrrci ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    }
    pc+=4;  
}
/**
 * exec_ebreak is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * The program is also halted if ebreak is found.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
        std::string s = render_ebreak(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// HALT";
    }
    halt = true;
    halt_reason = "EBREAK instruction";
}
/**
 * exec_ecall is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_ecall(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
        std::string s = rv32i_decode::render_ecall(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    }
    pc+=4;  
}
/**
 * exec_beq is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos)
{
    uint32_t imm_b = rv32i_decode::get_imm_b(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    //uint32_t val = pc +((rs1==rs2) ? imm_b : 4);
    uint32_t val = pc +((regs.get(rs1)==regs.get(rs2)) ? imm_b : 4);
    if(pos)
    {
        std::string s = rv32i_decode::render_btype(pc, insn, "beq    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " <<  hex::to_hex0x32(val); 
    }

    pc= val;  
}
/**
 * exec_bne is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos)
{
    uint32_t imm_b = rv32i_decode::get_imm_b(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    //uint32_t val = pc +((rs1!=rs2) ? imm_b : 4);
    uint32_t val = pc +((regs.get(rs1)!=regs.get(rs2)) ? imm_b : 4);
    if(pos)
    {
        std::string s = rv32i_decode::render_btype(pc, insn, "bne    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " <<  hex::to_hex0x32(val); 
        
    }
    pc = val;  
}
/**
 * exec_blt is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos)
{
    uint32_t imm_b = rv32i_decode::get_imm_b(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    //uint32_t val = pc +((rs1<rs2) ? imm_b : 4);
    uint32_t val = pc +((regs.get(rs1)<regs.get(rs2)) ? imm_b : 4);
    if(pos)
    {
        std::string s = rv32i_decode::render_btype(pc, insn, "blt    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " <<  hex::to_hex0x32(val); 
    }
    
    pc= val;    
}
/**
 * exec_bge is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos)
{
    uint32_t imm_b = rv32i_decode::get_imm_b(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    uint32_t val = pc +((regs.get(rs1)>=regs.get(rs2)) ? imm_b : 4);
    if(pos)
    {
        std::string s = rv32i_decode::render_btype(pc, insn, "bge    ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " <<  hex::to_hex0x32(val); 
    }
    pc= val;   
}
/**
 * exec_bltu is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos)
{
    uint32_t imm_b = rv32i_decode::get_imm_b(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    uint32_t val = pc +((rs1<rs2) ? imm_b : 4);
    //uint32_t val = pc +((regs.get(rs1)<regs.get(rs2)) ? imm_b : 4);
    if(pos)
    {
        std::string s = rv32i_decode::render_btype(pc, insn, "bltu   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc <U (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " <<  hex::to_hex0x32(val); 
    }
    pc= val;   
}
/**
 * exec_bgeu is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos)
{
    uint32_t imm_b = rv32i_decode::get_imm_b(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t rs2 = rv32i_decode::get_rs2(insn);
    uint32_t val = pc +((rs1>=rs2) ? imm_b : 4);
    //uint32_t val = pc +((regs.get(rs1)>=regs.get(rs2)) ? imm_b : 4);
    if(pos)
    {
        std::string s = rv32i_decode::render_btype(pc, insn, "bgeu   ");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc >=U (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " <<  hex::to_hex0x32(val); 
    }

    pc= val;   
}
/**
 * exec_addi is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t val = regs.get(rs1) + imm_i;

    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "addi   ", rv32i_decode::get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_slti is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t val = ((regs.get(rs1)< regs.get(imm_i)) ? 1 : 0);
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "slti   ", rv32i_decode::get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_sltiu is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t val = (((uint32_t)regs.get(rs1) < imm_i) ? 1 : 0);
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "sltiu  ", rv32i_decode::get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_xori is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t val = (regs.get(rs1) ^ (imm_i));
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "xori   ", rv32i_decode::get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_ori is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t val = (regs.get(rs1) | (imm_i));
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "ori    ", rv32i_decode::get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_andi is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t val = (regs.get(rs1) & (imm_i));
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "andi   ", rv32i_decode::get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_slli is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t shamt_i = imm_i % XLEN;
    uint32_t val = (regs.get(rs1)) << shamt_i;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "slli   ", rv32i_decode::get_imm_i(insn)%XLEN);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " << " << shamt_i << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_srai is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t shamt_i = imm_i % XLEN;
    uint32_t val = (regs.get(rs1)) >> shamt_i;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "srai   ", rv32i_decode::get_imm_i(insn)%XLEN);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " >> " << shamt_i << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}
/**
 * exec_srli is for executing the same type of instruction.  The instruction is also decoded if instructions are on.
 *
 * @param x insn is the insn code give for each instruction.
 *
 * @param y pos is the output stream, if give a non-null one, this instruction will then give instructions.
 *
 ********************************************************************************/
void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = rv32i_decode::get_rd(insn);
    uint32_t rs1 = rv32i_decode::get_rs1(insn);
    uint32_t imm_i = rv32i_decode::get_imm_i(insn);
    uint32_t shamt_i = imm_i % XLEN;
    uint32_t val = ((uint32_t)(regs.get(rs1))) >> shamt_i;
    if(pos)
    {
        std::string s = rv32i_decode::render_itype_alu(insn, "srli   ", rv32i_decode::get_imm_i(insn)%XLEN);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << rv32i_decode::render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " >> " << shamt_i << " = " << hex::to_hex0x32(val); 
    }
    regs.set(rd, val);
    pc+=4;  
}



