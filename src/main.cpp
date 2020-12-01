#include "bus.hpp"
#include "cpu6502.hpp"

int main(int argc, char** argv){
    Bus bus;
    CPU6502 cpu;
    cpu.ConnectBus(&bus);
    return 0;
}