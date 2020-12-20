#pragma once

/*
    References:
        official doc        http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf
        registers           http://www.obelisk.me.uk/6502/registers.html
        Addressing Modes    https://wiki.nesdev.com/w/index.php/CPU_addressing_modes http://www.obelisk.me.uk/6502/addressing.html 
        OpCodes:            http://www.obelisk.me.uk/6502/instructions.html
        Unimplemnted        http://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
*/

#include <cstdint>
#include <vector>
#include <string>
#include "common.hpp"

class Bus;

class CPU6502{
    public:
        CPU6502();
        ~CPU6502();

        enum FLAG{C,Z,I,D,B,U,V,N};

        struct Register{
            u16 PC;                // program counter
            u8 SP;                 // stack pointer
            u8 A;                  // Accumulator     
            u8 X;                  // X index
            u8 Y;                  // Y index
                        
            union{
                struct{
                    u8 C : 1;      // Carry
                    u8 Z : 1;      // Zero
                    u8 I : 1;      // Interrupt
                    u8 D : 1;      // Decimal
                    u8 U : 1;      // Unused
                    u8 B : 1;      // Break
                    u8 V : 1;      // Overflow
                    u8 N : 1;      // Negative
                }Flags;
                u8 buffer;
            }Status;
        };

        void ConnectBus(Bus *b) {bus = b;}
        u8 read (uint64_t addr);
        void write (u64 addr, u8 data);

        // Addressing Modes 
        u8 IMP();   u8 IMM();
        u8 ZP0();   u8 ZPX();
        u8 ZPY();   u8 REL();
        u8 ABS();   u8 ABX();
        u8 ABY();   u8 IND();
        u8 IZX();   u8 IZY();

        // Load/Store
        u8 LDA();   u8 LDX();   u8 LDY();  
        u8 STA();   u8 STX();   u8 STY();   
        // Register Transfers
        u8 TAX();   u8 TAY();   u8 TXA();   u8 TYA();   
        // Stack Operators
        u8 TSX();   u8 TXS();   u8 PHA();
        u8 PHP();   u8 PLA();   u8 PLP();
        // Logical
        u8 AND();   u8 EOR();   u8 ORA();   u8 BIT();
        // Arithmetic
        u8 ADC();   u8 SBC();   u8 CMP();   u8 CPX();   u8 CPY();
        // Inc/Dec
        u8 INC();   u8 INX();   u8 INY();
        u8 DEC();   u8 DEX();   u8 DEY();
        // Shifts
        u8 ASL();   u8 LSR();   u8 ROL(); u8 ROR();
        // Jumps and Calls
        u8 JMP();   u8 JSR();   u8 RTS();
        // Branches
        u8 BCC();   u8 BCS();   u8 BEQ();   u8 BMI();
        u8 BNE();   u8 BPL();   u8 BVC();   u8 BVS();
        // Status Flag Changes
        u8 CLC();   u8 CLD();   u8 CLI();   u8 CLV();   u8 SEC();   u8 SED();   u8 SEI();
        // Sys Functions
        u8 BRK();   u8 NOP();   u8 RTI();
        // Illegal opcode not implemented 
        u8 XXX(); 

        // async input signals
        // will allow current instructions to complete before interrupt
        void clock();               
        void reset();  
        void irq();             // interrupt request sig 
        void nmi();             // nonmaskable interrupt sig

        u8 fetch();
        u8 fetched = 0x00;
        
        u16 address_rel = 0x0000;
        u16 address_abs = 0x0000;
        u8 opcode = 0x00;
        u8 cycles = 0;

        struct INSTRUCTION
        {
            std::string name;
            u8(CPU6502::*operation) (void) = nullptr;
            u8(CPU6502::*addressingMode) (void) = nullptr;
            u8 cycles = 0;
        };

        std::vector<INSTRUCTION> lookup;

    private:
        Bus* bus = nullptr;
        Register registers;
        u8 getFlag(FLAG f);
        void setFlag(FLAG f, bool v);
};