#include "bus.h"

Bus::Bus(){
    for(auto &i : ram){ i = 0x00; }
    cpu.ConnectBus(this);
    cpu.reset();
}

Bus::~Bus(){}

void Bus::write(u16 addr, u8 data){
    if(addr >= 0x000 && addr <= 0xFFFF){
        ram[addr] = data;
    }
}

u8 Bus::read(u16 addr, bool readOnly){
    if(addr >= 0x000 && addr <= 0xFFFF){
        return ram[addr];
    }
    return 0x00;
}