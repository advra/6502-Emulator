#include "cpu6502.h"
#include "bus.h"

CPU6502::CPU6502(){ }

CPU6502::~CPU6502() { }

uint8_t CPU6502::read(uint64_t addr){
    return bus->read(addr);
}

void CPU6502::write(uint64_t addr, uint8_t data){
    bus->write(addr, data);
}

void CPU6502::reset(){
    registers.PC = 0xFFFC;
    registers.SP = 0x00;
    registers.A = registers.X = registers.Y = 0;
    registers.Status.buffer = 0x00;
}

uint8_t CPU6502::getFlag(FLAG f){
    return 0x00;
}

void CPU6502::setFlag(FLAG f, bool v){
    // registers.Status.buffer
}