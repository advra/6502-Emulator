#pragma once

#include <cstdint>
#include "common.hpp"

// Forward declaration of generic communications bus class to
// prevent circular inclusions
class Bus;

class CPU6502{
    public:
        CPU6502();
        ~CPU6502();

        enum FLAG{
            C = (1<<0),
            Z = (1<<1),
            I = (1<<2),
            D = (1<<3),
            B = (1<<4),
            U = (1<<5),
            V = (1<<6),
            N = (1<<7),
        };

        // registers http://www.obelisk.me.uk/6502/registers.html
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
        // https://wiki.nesdev.com/w/index.php/CPU_addressing_modes http://www.obelisk.me.uk/6502/addressing.html 
        u8 IMP();  u8 IMM();
        u8 ZP0();  u8 ZPX();
        u8 ZPY();  u8 REL();
        u8 ABS();  u8 ABX();
        u8 ABY();  u8 IND();
        u8 IZX();  u8 IZY();

        // opcodes http://www.obelisk.me.uk/6502/instructions.html
        // LDA();

        u8 UOF(); // unofficial opcodes that we do not implement http://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes

        void clock();
        // inputs
        void reset();
        void irq();         
        void nmi();         // non-maskable interrupts 

        u8 fetch();
        u8 fetched = 0x00;
        u16 address_abs = 0x0000;
        u8 cycles = 0;

    private:
        Bus* bus = nullptr;
        Register registers;
        u8 getFlag(FLAG f);
        void setFlag(FLAG f, bool v);
};