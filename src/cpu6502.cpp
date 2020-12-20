#include "cpu6502.hpp"
#include "bus.hpp"

// lookup code matrix: Pg 10 of http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf 
CPU6502::CPU6502()
{ 
    using a = CPU6502;
    lookup = 
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU6502::~CPU6502() { }

u8 CPU6502::read(u64 addr)
{
    return bus->read(addr);
}

void CPU6502::write(u64 addr, u8 data)
{
    bus->write(addr, data);
}
void CPU6502::reset()
{
	// Get address to set program counter to
	address_abs = 0xFFFC;
	uint16_t lo = read(address_abs + 0);
	uint16_t hi = read(address_abs + 1);

	// Set it
	registers.PC = (hi << 8) | lo;

	// Reset internal registers
	registers.A = 0;
	registers.X = 0;
	registers.Y = 0;
	registers.SP = 0xFD;
	registers.Status.buffer = 0x00 | FLAG::U;

	// Clear internal helper variables
	address_rel = 0x0000;
	address_abs = 0x0000;
	fetched = 0x00;

	// Reset takes time
	cycles = 8;
}

u8 CPU6502::getFlag(FLAG f)
{ 
    return registers.Status.buffer & (1 << f); 
}

void CPU6502::setFlag(FLAG f, bool v)
{
    if(v)
        registers.Status.buffer |= (1 << f);
    else
        registers.Status.buffer &= ~(1 << f);
}

void CPU6502::clock()
{
    if(cycles == 0)
    {
        opcode = read(registers.PC);
        registers.PC++;

        cycles = lookup[opcode].cycles;
        u8 additional_cycle1 = (this->*lookup[opcode].addressingMode) ();
        u8 additional_cycle2 = (this->*lookup[opcode].operation) ();

        cycles += (additional_cycle1 & additional_cycle2);
    }
}

// Addressing Modes
u8 CPU6502::IMP()
{
    fetched = registers.A;
    return 0;
}

u8 CPU6502::IMM()
{
    address_abs = registers.PC++;
    return 0;
}

u8 CPU6502::ZP0()
{
    address_abs = read(registers.PC);
    registers.PC++;
    address_abs &= 0x00FF;
    return 0;
}

u8 CPU6502::ZPX()
{
    address_abs = (read(registers.PC) + registers.X);
    registers.PC++;
    address_abs &= 0x00FF;
    return 0;
}

u8 CPU6502::ZPY()
{
    address_abs = (read(registers.PC) + registers.Y);
    registers.PC++;
    address_abs &= 0x00FF;
    return 0;
}

u8 CPU6502::ABS()
{
    u16 lo = read(registers.PC);
    registers.PC++;
    u16 hi = read(registers.PC);
    registers.PC++;

    address_abs = (hi << 8) | lo;
    return 0;
}

u8 CPU6502::ABX()
{
    u16 lo = read(registers.PC);
    registers.PC++;
    u16 hi = read(registers.PC);
    registers.PC++;

    address_abs = (hi << 8) | lo;
    address_abs += registers.X;

    // overflow check, change page
    if((address_abs & 0xFF00) != (hi << 8))
        return 1;
    else 
        return 0;
}

u8 CPU6502::ABY()
{
    u16 lo = read(registers.PC);
    registers.PC++;
    u16 hi = read(registers.PC);
    registers.PC++;

    address_abs = (hi << 8) | lo;
    address_abs += registers.Y;

    // overflow check, change page
    if((address_abs & 0xFF00) != (hi << 8))
        return 1;
    else 
        return 0;
}

u8 CPU6502::IND()
{
    u16 ptr_lo = read(registers.PC);
    registers.PC++;
    u16 ptr_hi = read(registers.PC);
    registers.PC++;

    u16 ptr =(ptr_hi << 8) | ptr_lo;
    address_abs = (read(ptr + 1) << 8 | read(ptr + 0));

    return 0;
}

u8 CPU6502::IZX()
{
    u16 t = read(registers.PC);
    registers.PC++;

    u16 lo = read((u16)(t + (u16) registers.X) & 0x00FF);
    u16 hi = read((u16)(t + (u16) registers.X + 1) & 0x00FF);

    address_abs = (hi << 8) | lo;

    return 0;
}

u8 CPU6502::IZY()
{
    u16 t = read(registers.PC);
    registers.PC++;

    u16 lo = read(t & 0x00FF);
    u16 hi = read((t+1) & 0x00FF);

    address_abs = (hi << 8) | lo;
    address_abs += registers.Y;

    if((address_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

u8 CPU6502::REL()
{
    address_rel = read(registers.PC);
    registers.PC++;

    if(address_rel & 0x80)
        address_rel |= 0xFF00;
    return 0;
}

// instructions
u8 CPU6502::fetch()
{
    if(!(lookup[opcode].addressingMode == &CPU6502::IMP))
        fetched = read(address_abs);
    return fetched;
}

u8 CPU6502::AND()
{
    fetch();
    registers.A &= fetched;
    setFlag(FLAG::Z, registers.A == 0x00);
    setFlag(FLAG::N, registers.A == 0x80);
    return 1;
}

u8 CPU6502::BCS()
{
    if(getFlag(C) == 1)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::BCC()
{
    if(getFlag(C) == 0)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::BEQ()
{
    if(getFlag(Z) == 0)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::BMI()
{
    if(getFlag(N) == 0)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::BNE()
{
    if(getFlag(Z) == 0)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::BPL()
{
    if(getFlag(N) == 0)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::BVC()
{
    if(getFlag(V) == 0)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::BVS()
{
    if(getFlag(V) == 1)
    {
        cycles++;
        address_abs = registers.PC + address_rel;

        // if branch needs to cross page boundary
        if((address_abs & 0xFF00) != (registers.PC & 0xFF00)) cycles++;

        registers.PC = address_abs;
    }   

    return 0;
}

u8 CPU6502::CLC()
{
    setFlag(C, false);
    return 0;
}

u8 CPU6502::CLD()
{
    setFlag(D, false);
    return 0;
}

u8 CPU6502::ADC()
{
    fetch();
    u16 temp = (u16) registers.A + (u16) fetched + (u16) getFlag(FLAG::C);
    setFlag(FLAG::C, temp > 255);
    setFlag(FLAG::Z, (temp & 0x00FF) == 0);
    setFlag(FLAG::N, temp > 0x80);
    setFlag(FLAG::V, (~((uint16_t)registers.A ^ (uint16_t)fetched) & ((uint16_t)registers.A ^ (uint16_t)temp)) & 0x0080);
    registers.A = temp & 0x00FF;
    return 1;
}

u8 CPU6502::SBC()
{
	fetch();
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
	
    u16 temp = (u16) registers.A + (u16) fetched + (u16) getFlag(FLAG::C);
    setFlag(FLAG::C, temp > 255);
    setFlag(FLAG::Z, (temp & 0x00FF) == 0);
    setFlag(FLAG::N, temp > 0x80);
    setFlag(FLAG::V, (~((uint16_t)registers.A ^ (uint16_t)fetched) & ((uint16_t)registers.A ^ (uint16_t)temp)) & 0x0080);
    registers.A = temp & 0x00FF;
    return 1;
}

