#pragma once

#include "cpu6502.hpp"
#include "common.hpp"
#include <cstdint>
#include <array>

class Bus{
    public:
        Bus();
        ~Bus();

        // devices on bus
        CPU6502 cpu;
        std::array<u8, 64 * SIZES::KB> ram; 

        void write(u16 addr, u8 data);
        u8 read(u16 addr, bool readOnly = false);
};