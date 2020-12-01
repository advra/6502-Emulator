#include "cpu6502.hpp"
#include "bus.hpp"

CPU6502::CPU6502(){ }

CPU6502::~CPU6502() { }

u8 CPU6502::read(u64 addr){
    return bus->read(addr);
}

void CPU6502::write(u64 addr, u8 data){
    bus->write(addr, data);
}

void CPU6502::reset(){
    address_abs = 0xFFFC;
    registers.SP = 0xFD;
    registers.A = registers.X = registers.Y = 0;
    registers.Status.buffer = 0x00 | FLAG::U;
    cycles = 0;
}

u8 CPU6502::getFlag(FLAG f){
    return 0x00;
}

void CPU6502::setFlag(FLAG f, bool v){
    // registers.Status.buffer
}