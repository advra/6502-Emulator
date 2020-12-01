#pragma once

#include <cstdint>

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
            uint16_t PC;                // program counter
            uint8_t SP;                 // stack pointer
            uint8_t A;                  // Accumulator     
            uint8_t X;                  // X index
            uint8_t Y;                  // Y index
                        
            union{
                struct{
                    uint8_t C : 1;      // Carry
                    uint8_t Z : 1;      // Zero
                    uint8_t I : 1;      // Interrupt
                    uint8_t D : 1;      // Decimal
                    uint8_t U : 1;      // Unused
                    uint8_t B : 1;      // Break
                    uint8_t V : 1;      // Overflow
                    uint8_t N : 1;      // Negative
                }Flags;
                uint8_t buffer;
            }Status;
        };

        void ConnectBus(Bus *b) {bus = b;}
        uint8_t read (uint64_t addr);
        void write (uint64_t addr, uint8_t data);
      

        // Addressing Modes 
        // https://wiki.nesdev.com/w/index.php/CPU_addressing_modes http://www.obelisk.me.uk/6502/addressing.html 
        uint8_t IMP();  uint8_t IMM();
        uint8_t ZP0();  uint8_t ZPX();
        uint8_t ZPY();  uint8_t REL();
        uint8_t ABS();  uint8_t ABX();
        uint8_t ABY();  uint8_t IND();
        uint8_t IZX();  uint8_t IZY();

        // opcodes http://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes

        uint8_t ILL(); //unimplemented opcode

        void clock();
        // inputs
        void reset();
        void irq();         
        void nmi();         // non-maskable interrupts

        uint8_t fetch();
        uint8_t fetched = 0x00;
        uint16_t address_abs = 0x0000;
        uint8_t cycles = 0;

    private:
        Bus* bus = nullptr;
        Register registers;
        uint8_t getFlag(FLAG f);
        void setFlag(FLAG f, bool v);
};