#pragma

#include"cpu6502.h"

uint8_t CPU6502::read(uint64_t addr){
    bus->read(addr);
}

void CPU6502::write(uint64_t addr, uint8_t data){
    bus->write(addr, data);

    Register r;
}