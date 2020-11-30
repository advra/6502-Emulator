#pragma once

#include "bus.h"

class CPU6502{
    
        // registers http://www.obelisk.me.uk/6502/registers.html
        typedef struct Register{
            uint8_t PC;                 // program counter
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
                    uint8_t B : 1;      // Break
                    uint8_t V : 1;      // Overflow
                    uint8_t N : 1;      // Negative
                }Flags;
                uint8_t buf;
            }Status;
        };

    public:
        CPU6502();
        ~CPU6502();

        void ConnectBus(Bus *b) {bus = b;}
        uint8_t read (uint64_t addr);
        void write (uint64_t addr, uint8_t data);
        uint8_t getFlag();
        void setFlag();

        // Addressing Modes http://www.obelisk.me.uk/6502/addressing.html

        // opcodes http://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes

        // clock
        void clock();
        void reset();
        void irq();     
        void nmi();                     // non-maskable interrupts

    private:
        Bus *bus = nullptr;
        Register registers;
};