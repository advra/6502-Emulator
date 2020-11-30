#pragma

#include"cpu6502.h"

uint8_t CPU6502::read(uint64_t addr){
    bus->read(addr);
}

void CPU6502::write(uint64_t addr, uint8_t data){
    bus->write(addr, data);
}

void CPU6502::reset(){
    registers.PC = 0xFFFC;
    registers.SP = 0x0100;
    registers.A = registers.X = registers.Y = 0;
    registers.Status.buf = 0x00;
}