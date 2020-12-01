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
    // clear internal helper vars
    address_rel = address_abs = 0x0000;
    fetched = 0x00;
    // reset takes time
    cycles = 8;
}

u8 CPU6502::getFlag(FLAG f){
    return registers.Status.buffer & (1 << f);
}

void CPU6502::setFlag(FLAG f, bool v){
    if(v==1) registers.Status.buffer |= (1 << f);
    registers.Status.buffer &= ~(1 << f);
}