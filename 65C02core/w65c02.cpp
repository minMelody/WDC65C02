#include "w65c02.h"
#include "Bus.h"

#define LOG_INFO

WDC65C02::w65c02::w65c02(Bus* bus)
{
	this->bus = bus;
	opsLUT =
	{	//                0                                  1                                  2                                  3                                  4                                  5                                  6                                  7                                  8                                  9                                  A                                  B                                  C                                  D                                  E                                  F
/*0*/	{&w65c02::BRK, &w65c02::IMM, 7, 0},{&w65c02::ORA, &w65c02::ZIX, 6, 0},{&w65c02::NOP, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::TSB, &w65c02::ZP , 5, 0},{&w65c02::ORA, &w65c02::ZP , 3, 0},{&w65c02::ASL, &w65c02::ZP , 5, 0},{&w65c02::RMB0, &w65c02::ZP, 5, 0},{&w65c02::PHP, &w65c02::IMP, 3, 0},{&w65c02::ORA, &w65c02::IMM, 2, 0},{&w65c02::ASL, &w65c02::ACC, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::TSB, &w65c02::ABS, 6, 0},{&w65c02::ORA, &w65c02::ABS, 4, 0},{&w65c02::ASL, &w65c02::ABS, 6, 0},{&w65c02::BBR0, &w65c02::ZPR, 5, 0},
/*1*/	{&w65c02::BPL, &w65c02::REL, 2, 0},{&w65c02::ORA, &w65c02::ZIY, 5, 1},{&w65c02::ORA, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::TRB, &w65c02::ZP , 5, 0},{&w65c02::ORA, &w65c02::ZPX, 4, 0},{&w65c02::ASL, &w65c02::ZPX, 6, 0},{&w65c02::RMB1, &w65c02::ZP, 5, 0},{&w65c02::CLC, &w65c02::IMP, 2, 0},{&w65c02::ORA, &w65c02::ABY, 4, 1},{&w65c02::INC, &w65c02::ACC, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::TRB, &w65c02::ABS, 6, 0},{&w65c02::ORA, &w65c02::ABX, 4, 1},{&w65c02::ASL, &w65c02::ABX, 6, 1},{&w65c02::BBR1, &w65c02::ZPR, 5, 0},
/*2*/	{&w65c02::JSR, &w65c02::ABS, 6, 0},{&w65c02::AND, &w65c02::ZIX, 6, 0},{&w65c02::NOP, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::BIT, &w65c02::ZP , 3, 0},{&w65c02::AND, &w65c02::ZP , 3, 0},{&w65c02::ROL, &w65c02::ZP , 5, 0},{&w65c02::RMB2, &w65c02::ZP, 5, 0},{&w65c02::PLP, &w65c02::IMP, 4, 0},{&w65c02::AND, &w65c02::IMM, 2, 0},{&w65c02::ROL, &w65c02::ACC, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::BIT, &w65c02::ABS, 4, 0},{&w65c02::AND, &w65c02::ABS, 4, 0},{&w65c02::ROL, &w65c02::ABS, 6, 0},{&w65c02::BBR2, &w65c02::ZPR, 5, 0},
/*3*/	{&w65c02::BMI, &w65c02::REL, 2, 0},{&w65c02::AND, &w65c02::ZIY, 5, 1},{&w65c02::AND, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::BIT, &w65c02::ZPX, 4, 0},{&w65c02::AND, &w65c02::ZPX, 4, 0},{&w65c02::ROL, &w65c02::ZPX, 6, 0},{&w65c02::RMB3, &w65c02::ZP, 5, 0},{&w65c02::SEC, &w65c02::IMP, 2, 0},{&w65c02::AND, &w65c02::ABY, 4, 1},{&w65c02::DEC, &w65c02::ACC, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::BIT, &w65c02::ABX, 4, 1},{&w65c02::AND, &w65c02::ABX, 4, 1},{&w65c02::ROL, &w65c02::ABX, 6, 1},{&w65c02::BBR3, &w65c02::ZPR, 5, 0},
/*4*/	{&w65c02::RTI, &w65c02::IMP, 6, 0},{&w65c02::EOR, &w65c02::ZIX, 6, 0},{&w65c02::NOP, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::NOP, &w65c02::ZP , 3, 0},{&w65c02::EOR, &w65c02::ZP , 3, 0},{&w65c02::LSR, &w65c02::ZP , 5, 0},{&w65c02::RMB4, &w65c02::ZP, 5, 0},{&w65c02::PHA, &w65c02::IMP, 3, 0},{&w65c02::EOR, &w65c02::IMM, 2, 0},{&w65c02::LSR, &w65c02::ACC, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::JMP, &w65c02::ABS, 3, 0},{&w65c02::EOR, &w65c02::ABS, 4, 0},{&w65c02::LSR, &w65c02::ABS, 6, 0},{&w65c02::BBR4, &w65c02::ZPR, 5, 0},
/*5*/	{&w65c02::BVC, &w65c02::REL, 2, 0},{&w65c02::EOR, &w65c02::ZIY, 5, 1},{&w65c02::EOR, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::NOP, &w65c02::ZPX, 4, 0},{&w65c02::EOR, &w65c02::ZPX, 4, 0},{&w65c02::LSR, &w65c02::ZPX, 6, 0},{&w65c02::RMB5, &w65c02::ZP, 5, 0},{&w65c02::CLI, &w65c02::IMP, 2, 0},{&w65c02::EOR, &w65c02::ABY, 4, 1},{&w65c02::PHY, &w65c02::IMP, 3, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::NOP, &w65c02::ABS, 8, 0},{&w65c02::EOR, &w65c02::ABX, 4, 1},{&w65c02::LSR, &w65c02::ABX, 6, 1},{&w65c02::BBR5, &w65c02::ZPR, 5, 0},
/*6*/	{&w65c02::RTS, &w65c02::IMP, 6, 0},{&w65c02::ADC, &w65c02::ZIX, 6, 0},{&w65c02::NOP, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::STZ, &w65c02::ZP , 3, 0},{&w65c02::ADC, &w65c02::ZP , 3, 0},{&w65c02::ROR, &w65c02::ZP , 5, 0},{&w65c02::RMB6, &w65c02::ZP, 5, 0},{&w65c02::PLA, &w65c02::IMP, 4, 0},{&w65c02::ADC, &w65c02::IMM, 2, 0},{&w65c02::ROR, &w65c02::ACC, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::JMP, &w65c02::IND, 6, 0},{&w65c02::ADC, &w65c02::ABS, 4, 0},{&w65c02::ROR, &w65c02::ABS, 6, 0},{&w65c02::BBR6, &w65c02::ZPR, 5, 0},
/*7*/	{&w65c02::BVS, &w65c02::REL, 2, 0},{&w65c02::ADC, &w65c02::ZIY, 5, 1},{&w65c02::ADC, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::STZ, &w65c02::ZPX, 4, 0},{&w65c02::ADC, &w65c02::ZPX, 4, 0},{&w65c02::ROR, &w65c02::ZPX, 6, 0},{&w65c02::RMB7, &w65c02::ZP, 5, 0},{&w65c02::SEI, &w65c02::IMP, 2, 0},{&w65c02::ADC, &w65c02::ABY, 4, 1},{&w65c02::PLY, &w65c02::IMP, 4, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::JMP, &w65c02::AIX, 6, 0},{&w65c02::ADC, &w65c02::ABX, 4, 1},{&w65c02::ROR, &w65c02::ABX, 6, 1},{&w65c02::BBR7, &w65c02::ZPR, 5, 0},
/*8*/	{&w65c02::BRA, &w65c02::REL, 3, 0},{&w65c02::STA, &w65c02::ZIX, 6, 0},{&w65c02::NOP, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::STY, &w65c02::ZP , 3, 0},{&w65c02::STA, &w65c02::ZP , 3, 0},{&w65c02::STX, &w65c02::ZP , 3, 0},{&w65c02::SMB0, &w65c02::ZP, 5, 0},{&w65c02::DEY, &w65c02::IMP, 2, 0},{&w65c02::BIT, &w65c02::IMM, 2, 0},{&w65c02::TXA, &w65c02::IMP, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::STY, &w65c02::ABS, 4, 0},{&w65c02::STA, &w65c02::ABS, 4, 0},{&w65c02::STX, &w65c02::ABS, 4, 0},{&w65c02::BBS0, &w65c02::ZPR, 5, 0},
/*9*/	{&w65c02::BCC, &w65c02::REL, 2, 0},{&w65c02::STA, &w65c02::ZIY, 6, 0},{&w65c02::STA, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::STY, &w65c02::ZPX, 4, 0},{&w65c02::STA, &w65c02::ZPX, 4, 0},{&w65c02::STX, &w65c02::ZPX, 4, 0},{&w65c02::SMB1, &w65c02::ZP, 5, 0},{&w65c02::TYA, &w65c02::IMP, 2, 0},{&w65c02::STA, &w65c02::ABY, 5, 0},{&w65c02::TXS, &w65c02::IMP, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::STZ, &w65c02::ABS, 4, 0},{&w65c02::STA, &w65c02::ABX, 5, 0},{&w65c02::STZ, &w65c02::ABX, 5, 0},{&w65c02::BBS1, &w65c02::ZPR, 5, 0},
/*A*/	{&w65c02::LDY, &w65c02::IMM, 2, 0},{&w65c02::LDA, &w65c02::ZIX, 6, 0},{&w65c02::LDX, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::LDY, &w65c02::ZP , 3, 0},{&w65c02::LDA, &w65c02::ZP , 3, 0},{&w65c02::LDX, &w65c02::ZP , 3, 0},{&w65c02::SMB2, &w65c02::ZP, 5, 0},{&w65c02::TAY, &w65c02::IMP, 2, 0},{&w65c02::LDA, &w65c02::IMM, 2, 0},{&w65c02::TAX, &w65c02::IMP, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::LDY, &w65c02::ABS, 4, 0},{&w65c02::LDA, &w65c02::ABS, 4, 0},{&w65c02::LDX, &w65c02::ABS, 4, 0},{&w65c02::BBS2, &w65c02::ZPR, 5, 0},
/*B*/	{&w65c02::BCS, &w65c02::REL, 2, 0},{&w65c02::LDA, &w65c02::ZIY, 5, 1},{&w65c02::LDA, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::LDY, &w65c02::ZPX, 4, 0},{&w65c02::LDA, &w65c02::ZPX, 4, 0},{&w65c02::LDX, &w65c02::ZPY, 4, 0},{&w65c02::SMB3, &w65c02::ZP, 5, 0},{&w65c02::CLV, &w65c02::IMP, 2, 0},{&w65c02::LDA, &w65c02::ABY, 4, 1},{&w65c02::TSX, &w65c02::IMP, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::LDY, &w65c02::ABX, 4, 1},{&w65c02::LDA, &w65c02::ABX, 4, 1},{&w65c02::LDX, &w65c02::ABY, 4, 1},{&w65c02::BBS3, &w65c02::ZPR, 5, 0},
/*C*/	{&w65c02::CPY, &w65c02::IMM, 2, 0},{&w65c02::CMP, &w65c02::ZIX, 6, 0},{&w65c02::NOP, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::CPY, &w65c02::ZP , 3, 0},{&w65c02::CMP, &w65c02::ZP , 3, 0},{&w65c02::DEC, &w65c02::ZP , 5, 0},{&w65c02::SMB4, &w65c02::ZP, 5, 0},{&w65c02::INY, &w65c02::IMP, 2, 0},{&w65c02::CMP, &w65c02::IMM, 2, 0},{&w65c02::DEX, &w65c02::IMP, 2, 0},{&w65c02::WAI, &w65c02::IMP, 3, 0},{&w65c02::CPY, &w65c02::ABS, 4, 0},{&w65c02::CMP, &w65c02::ABS, 4, 0},{&w65c02::DEC, &w65c02::ABS, 6, 0},{&w65c02::BBS4, &w65c02::ZPR, 5, 0},
/*D*/	{&w65c02::BNE, &w65c02::REL, 2, 0},{&w65c02::CMP, &w65c02::ZIY, 5, 1},{&w65c02::CMP, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::NOP, &w65c02::ZPX, 4, 0},{&w65c02::CMP, &w65c02::ZPX, 4, 0},{&w65c02::DEC, &w65c02::ZPX, 6, 0},{&w65c02::SMB5, &w65c02::ZP, 5, 0},{&w65c02::CLD, &w65c02::IMP, 2, 0},{&w65c02::CMP, &w65c02::ABY, 4, 1},{&w65c02::PHX, &w65c02::IMP, 3, 0},{&w65c02::STP, &w65c02::IMP, 3, 0},{&w65c02::NOP, &w65c02::ABS, 4, 0},{&w65c02::CMP, &w65c02::ABX, 4, 1},{&w65c02::DEC, &w65c02::ABX, 7, 0},{&w65c02::BBS5, &w65c02::ZPR, 5, 0},
/*E*/	{&w65c02::CPX, &w65c02::IMM, 2, 0},{&w65c02::SBC, &w65c02::ZIX, 6, 0},{&w65c02::NOP, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::CPX, &w65c02::ZP , 3, 0},{&w65c02::SBC, &w65c02::ZP , 3, 0},{&w65c02::INC, &w65c02::ZP , 5, 0},{&w65c02::SMB6, &w65c02::ZP, 5, 0},{&w65c02::INX, &w65c02::IMP, 2, 0},{&w65c02::SBC, &w65c02::IMM, 2, 0},{&w65c02::NOP, &w65c02::IMP, 2, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::CPX, &w65c02::ABS, 4, 0},{&w65c02::SBC, &w65c02::ABS, 4, 0},{&w65c02::INC, &w65c02::ABS, 6, 0},{&w65c02::BBS6, &w65c02::ZPR, 5, 0},
/*F*/	{&w65c02::BEQ, &w65c02::REL, 2, 0},{&w65c02::SBC, &w65c02::ZIY, 5, 1},{&w65c02::SBC, &w65c02::ZPI, 5, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::NOP, &w65c02::ZPX, 4, 0},{&w65c02::SBC, &w65c02::ZPX, 4, 0},{&w65c02::INC, &w65c02::ZPX, 6, 0},{&w65c02::SMB7, &w65c02::ZP, 5, 0},{&w65c02::SED, &w65c02::IMP, 2, 0},{&w65c02::SBC, &w65c02::ABY, 4, 1},{&w65c02::PLX, &w65c02::IMP, 4, 0},{&w65c02::NOP, &w65c02::IMP, 1, 0},{&w65c02::NOP, &w65c02::ABS, 4, 0},{&w65c02::SBC, &w65c02::ABX, 4, 1},{&w65c02::INC, &w65c02::ABX, 7, 0},{&w65c02::BBS7, &w65c02::ZPR, 5, 0},
	};

}

WDC65C02::w65c02::~w65c02()
{
	/* void */
}

void WDC65C02::w65c02::step()
{
	if (stop) return;

	if (cycles == 0)
	{
		if (nmiPending)
		{
			nmi();
			wait = nmiPending = irqPending = false;
			return;
		}
		else if (irqPending)
		{
			irq();
			wait = nmiPending = irqPending = false;
			return;
		}
		else if (wait) return;

#ifdef LOG_INFO
		uint16_t insPC = PC;
#endif // LOG_INFO

		IR = bus->read(PC);
		PC++;

		cycles = opsLUT[IR].cycles;

		setFlag(_, 1); // unused flag is always set

		if (opsLUT[IR].fetch == &w65c02::ACC || opsLUT[IR].fetch == &w65c02::IMM || opsLUT[IR].fetch == &w65c02::IMP || opsLUT[IR].fetch == &w65c02::ZPR)
		{
			opd = (this->*opsLUT[IR].fetch)(opsLUT[IR].extraCycle) & 0x00FF;
		}
		else
		{
			fetchedAdr = (this->*opsLUT[IR].fetch)(opsLUT[IR].extraCycle);
			opd = bus->read(fetchedAdr);
		}

		(this->*opsLUT[IR].execute)();

#ifdef LOG_INFO
		printf("$%04X:%02X  A:%02X X:%02X Y:%02X SP:%02X SR:%c%c%c%c%c%c%c%c\n",
			insPC, IR, A, X, Y, SP,
			getFlag(N) ? 'N' : '.', getFlag(V) ? 'V' : '.', getFlag(_) ? '1' : '.',
			getFlag(B) ? 'B' : '.', getFlag(D) ? 'D' : '.', getFlag(I) ? 'I' : '.',
			getFlag(Z) ? 'Z' : '.', getFlag(C) ? 'C' : '.');
#endif // LOG_INFO

	}

	cycles--;
}

// ===== Hardware pins and Interrupts =====

void WDC65C02::w65c02::reset()
{
	stop = wait = irqPending = nmiPending = false;
	IR = opd = 0x00;
	temp.result_16 = fetchedAdr = 0x0000;

	// Reset internal registers
	A = X = Y = 0x00;
	SP = 0xFD;
	SR = 0x00 | Statusflags::_;

	// Fetch the reset vector
	uint8_t pcl = bus->read(0xFFFC);
	uint8_t pch = bus->read(0xFFFD);
	// Load it onto the program counter
	PC = (pch << 8) | pcl;

	cycles = 8;
}

// Interrupt
void WDC65C02::w65c02::irq()
{
	if (getFlag(I)) return;
	// Push the program counter and status register onto the stack
	stackPush((PC >> 8) & 0x00FF);
	stackPush(PC & 0x00FF);
	stackPush(SR);

	// Set interrupt disable flag
	setFlag(I, 1);
	setFlag(D, 0);

	// Fetch the interrupt handler vector
	uint8_t pcl = bus->read(0xFFFE);
	uint8_t pch = bus->read(0xFFFF);
	PC = (pch << 8) | pcl;

	cycles = 7;
}

// Non-Maskable Interrupt
void WDC65C02::w65c02::nmi()
{
	stackPush((PC >> 8) & 0x00FF);
	stackPush(PC & 0x00FF);
	stackPush(SR);

	// Set interrupt disable flag
	setFlag(I, 1);
	setFlag(D, 0);

	uint8_t pcl = bus->read(0xFFFA);
	uint8_t pch = bus->read(0xFFFB);
	PC = (pch << 8) | pcl;

	cycles = 7;
}

// In hardware: The RDY pin can be set low by the 65c02 to signal a wait-for-interrupt mode.
bool WDC65C02::w65c02::ready()
{
	return !(wait || stop);
}
// In hardware: The SYNC pin is high when the microprocessor is fetching an opcode.
bool WDC65C02::w65c02::sync()
{
	return cycles == 0;
}

// ===== Status Flags =====

bool WDC65C02::w65c02::getFlag(Statusflags f)
{
	return (SR & f) > 0;
}

void WDC65C02::w65c02::setFlag(Statusflags f, bool v)
{
	if (v) SR |= f;
	else SR &= ~f;
}

// ===== Stack =====
// The stack uses memory from page 1 ($0100 to $01FF), decrementing as bytes are pushed.

void WDC65C02::w65c02::stackPush(uint8_t byte)
{
	bus->write(0x0100 | SP, byte);
	SP--;
}
uint8_t WDC65C02::w65c02::stackPop()
{
	SP++;
	return bus->read(0x0100 | SP);
}

// ===== Addressing Modes =====

// Accumulator
uint16_t WDC65C02::w65c02::ACC(bool extraCycle)
{
	return A;
}
// Immediate
uint16_t WDC65C02::w65c02::IMM(bool extraCycle)
{
	uint8_t operand = bus->read(PC);
	PC++;
	return operand;
}
// Implied
uint16_t WDC65C02::w65c02::IMP(bool extraCycle)
{
	return IR;
}
// Absolute
uint16_t WDC65C02::w65c02::ABS(bool extraCycle)
{
	uint8_t lb = bus->read(PC);
	PC++;
	uint8_t hb = bus->read(PC);
	PC++;
	return (hb << 8) | lb;
}
// Absolute Indexed X
uint16_t WDC65C02::w65c02::ABX(bool extraCycle)
{
	uint8_t lb = bus->read(PC);
	PC++;
	uint8_t hb = bus->read(PC);
	PC++;
	uint16_t addr = (hb << 8) | lb;
	addr += X;

	if (extraCycle)
	{
		if ((addr & 0xFF00) != (hb << 8)) cycles++;
	}

	return addr;
}
// Absolute Indexed Y
uint16_t WDC65C02::w65c02::ABY(bool extraCycle)
{
	uint8_t lb = bus->read(PC);
	PC++;
	uint8_t hb = bus->read(PC);
	PC++;
	uint16_t addr = (hb << 8) | lb;
	addr += Y;

	if (extraCycle)
	{
		if ((addr & 0xFF00) != (hb << 8)) cycles++;
	}

	return addr;
}
// Indirect
// The 65c02 corrected the indirect addressing bug
uint16_t WDC65C02::w65c02::IND(bool extraCycle)
{
	uint8_t lptr = bus->read(PC);
	PC++;
	uint8_t hptr = bus->read(PC);
	PC++;
	uint16_t ptr = (hptr << 8) | lptr;

	uint8_t lb = bus->read(ptr);
	uint8_t hb = bus->read(ptr + 1);
	return (hb << 8) | lb;
}
// Relative
// Offset can be negative
uint16_t WDC65C02::w65c02::REL(bool extraCycle)
{
	int8_t offset = bus->read(PC);
	PC++;
	return PC + offset;
}
// Indexed Indirect (adr,X)
uint16_t WDC65C02::w65c02::ZIX(bool extraCycle)
{
	uint8_t ptr = bus->read(PC);
	PC++;
	ptr += X;

	uint8_t lb = bus->read(ptr);
	uint8_t hb = bus->read(ptr + 1);

	return (hb << 8) | lb;
}
// Indirect Indexed (adr),Y
uint16_t WDC65C02::w65c02::ZIY(bool extraCycle)
{
	uint8_t ptr = bus->read(PC);
	PC++;

	uint8_t lb = bus->read(ptr);
	uint8_t hb = bus->read(ptr + 1);

	uint16_t addr = (hb << 8) | lb;
	addr += Y;

	if (extraCycle)
	{
		if ((addr & 0xFF00) != (hb << 8)) cycles++;
	}

	return addr;
}
// Zero Page
uint16_t WDC65C02::w65c02::ZP (bool extraCycle)
{
	uint8_t addr = bus->read(PC);
	PC++;
	return addr;
}
// Zero Page Indexed X
uint16_t WDC65C02::w65c02::ZPX(bool extraCycle)
{
	uint8_t addr = bus->read(PC);
	PC++;
	// Taking advantage of addr being an 8 bit integer the carry is automatically discarded
	addr += X;
	return addr;
}
// Zero Page Indexed Y
uint16_t WDC65C02::w65c02::ZPY(bool extraCycle)
{
	uint8_t addr = bus->read(PC);
	PC++;
	addr += Y;
	return addr;
}

// ===== 65c02 Addressing Modes =====

// Absolute Indexed Indirect
uint16_t WDC65C02::w65c02::AIX(bool extraCycle)
{
	uint8_t lptr = bus->read(PC);
	PC++;
	uint8_t hptr = bus->read(PC);
	PC++;
	uint16_t ptr = (hptr << 8) | lptr;
	ptr += X;

	uint8_t lb = bus->read(ptr);
	uint8_t hb = bus->read(ptr + 1);
	return (hb << 8) | lb;
}
// Zero Page Indirect
uint16_t WDC65C02::w65c02::ZPI(bool extraCycle)
{
	uint8_t ptr = bus->read(PC);
	PC++;

	uint8_t lb = bus->read(ptr);
	uint8_t hb = bus->read(ptr + 1);

	return (hb << 8) | lb;
}
// Zero Page with Relative
uint16_t WDC65C02::w65c02::ZPR(bool extraCycle)
{
	uint16_t zp = this->ZP(extraCycle);
	fetchedAdr = this->REL(extraCycle);

	return bus->read(zp);
}

// ===== Instructions =====

// Conditional Branch: Flag
void WDC65C02::w65c02::BranchIf(Statusflags f, bool condition)
{
	if (getFlag(f) == condition)
	{
		cycles++;
		if ((PC & 0xFF00) != (fetchedAdr & 0xFF00)) cycles++;
		PC = fetchedAdr;
	}
}
// Conditional Branch: Bit
void WDC65C02::w65c02::BranchIf(uint8_t bit, bool condition)
{
	uint8_t mask = 1 << bit;
	if (((opd & mask) > 0) == condition)
	{
		cycles++;
		if ((PC & 0xFF00) != (fetchedAdr & 0xFF00)) cycles++;
		PC = fetchedAdr;
	}
}

// Add with carry
void WDC65C02::w65c02::ADC()
{
	uint8_t lo_nibble = (A & 0x0F) + (opd & 0x0F) + getFlag(C);
	uint8_t hi_nibble = (A >> 4) + (opd >> 4);
	temp.result_16 = (hi_nibble << 4) + lo_nibble;
	//x temp.result_8 = temp.result_16 & 0x00FF;

	setFlag(C, temp.result_16 & 0xFF00);

	if (getFlag(D))
	{
		if (lo_nibble > 9) lo_nibble += 6;
		hi_nibble += lo_nibble > 0x0F;

		if (hi_nibble > 9) hi_nibble += 6;
		setFlag(C, hi_nibble > 0x0F);

		temp.result_8 = (hi_nibble << 4) | (lo_nibble & 0x0F);
		// The 65C02 sets the flags after applying correction, at the cost of an extra cycle
		cycles++;
	}

	setFlag(N, temp.result_8 & 0x80);
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(V, (~(A ^ opd) & (A ^ temp.result_8)) & 0x80);

	A = temp.result_8;
}
// Logical AND
void WDC65C02::w65c02::AND()
{
	A &= opd;
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
}
// Arithmetic Shift Left
void WDC65C02::w65c02::ASL()
{
	temp.result_16 = opd << 1;
	//x temp.result_8 = temp.result_16 & 0x00FF;

	setFlag(C, temp.result_16 & 0xFF00);
	setFlag(N, temp.result_16 & 0x80);
	setFlag(Z, (temp.result_16 & 0x00FF) == 0x00);

	if (opsLUT[IR].fetch == &w65c02::ACC)
	{
		A = temp.result_8;
	}
	else
	{
		bus->write(fetchedAdr, temp.result_8);
	}
}
// Branch on carry clear
void WDC65C02::w65c02::BCC()
{
	BranchIf(C, 0);
}
// Branch on carry set
void WDC65C02::w65c02::BCS()
{
	BranchIf(C, 1);
}
// Branch on equal
void WDC65C02::w65c02::BEQ()
{
	BranchIf(Z, 1);
}
// Bit test
void WDC65C02::w65c02::BIT()
{
	temp.result_8 = A & opd;
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(V, temp.result_8 & (1 << 6));
	setFlag(N, temp.result_8 & (1 << 7));
}
// Branch if Negative
void WDC65C02::w65c02::BMI()
{
	BranchIf(N, 1);
}
// Branch on not equal
void WDC65C02::w65c02::BNE()
{
	BranchIf(Z, 0);
}
// Branch if Positive
void WDC65C02::w65c02::BPL()
{
	BranchIf(N, 0);
}
// Break
void WDC65C02::w65c02::BRK()
{
	PC++;

	stackPush((PC >> 8) & 0x00FF);
	stackPush(PC & 0x00FF);
	stackPush(SR | Statusflags::B);

	setFlag(I, 1);
	setFlag(D, 0);

	uint8_t pcl = bus->read(0xFFFE);
	uint8_t pch = bus->read(0xFFFF);
	PC = (pch << 8) | pcl;
}
// Branch on overflow clear
void WDC65C02::w65c02::BVC()
{
	BranchIf(V, 0);
}
// Branch on overflow set
void WDC65C02::w65c02::BVS()
{
	BranchIf(V, 1);
}
// Clear Carry flag
void WDC65C02::w65c02::CLC()
{
	setFlag(C, 0);
}
// Clear Decimal flag
void WDC65C02::w65c02::CLD()
{
	setFlag(D, 0);
}
// Clear Interrupt flag
void WDC65C02::w65c02::CLI()
{
	setFlag(I, 0);
}
// Clear overflow flag
void WDC65C02::w65c02::CLV()
{
	setFlag(V, 0);
}
// Compare accumulator
void WDC65C02::w65c02::CMP()
{
	setFlag(C, A >= opd);
	temp.result_8 = A - opd;
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(N, temp.result_8 & 0x80);
}
// Compare X
void WDC65C02::w65c02::CPX()
{
	setFlag(C, X >= opd);
	temp.result_8 = X - opd;
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(N, temp.result_8 & 0x80);
}
// Compare Y
void WDC65C02::w65c02::CPY()
{
	setFlag(C, Y >= opd);
	temp.result_8 = Y - opd;
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(N, temp.result_8 & 0x80);
}
// Decrement memory
void WDC65C02::w65c02::DEC()
{
	temp.result_8 = opd - 1;
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(N, temp.result_8 & 0x80);

	if (opsLUT[IR].fetch == &w65c02::ACC)
	{
		A = temp.result_8;
	}
	else
	{
		bus->write(fetchedAdr, temp.result_8);
	}
}
// Decrement X
void WDC65C02::w65c02::DEX()
{
	X--;
	setFlag(Z, X == 0x00);
	setFlag(N, X & 0x80);
}
// Decrement index Y
void WDC65C02::w65c02::DEY()
{
	Y--;
	setFlag(Z, Y == 0x00);
	setFlag(N, Y & 0x80);
}
// Exclusive Or
void WDC65C02::w65c02::EOR()
{
	A ^= opd;
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
}
// Increment memory
void WDC65C02::w65c02::INC()
{
	temp.result_8 = opd + 1;
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(N, temp.result_8 & 0x80);

	if (opsLUT[IR].fetch == &w65c02::ACC)
	{
		A = temp.result_8;
	}
	else
	{
		bus->write(fetchedAdr, temp.result_8);
	}
}
// Increment X
void WDC65C02::w65c02::INX()
{
	X++;
	setFlag(Z, X == 0x00);
	setFlag(N, X & 0x80);
}
// Increment Y
void WDC65C02::w65c02::INY()
{
	Y++;
	setFlag(Z, Y == 0x00);
	setFlag(N, Y & 0x80);
}
// Jump
void WDC65C02::w65c02::JMP()
{
	PC = fetchedAdr;
}
// Jump to subroutine
void WDC65C02::w65c02::JSR()
{
	PC--;
	stackPush((PC >> 8) && 0x00FF);
	stackPush(PC & 0x00FF);
	PC = fetchedAdr;
}
// Load Accumulator
void WDC65C02::w65c02::LDA()
{
	A = opd;
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
}
// Load index X
void WDC65C02::w65c02::LDX()
{
	X = opd;
	setFlag(Z, X == 0x00);
	setFlag(N, X & 0x80);
}
// Load index Y
void WDC65C02::w65c02::LDY()
{
	Y = opd;
	setFlag(Z, Y == 0x00);
	setFlag(N, Y & 0x80);
}
// Logical shift right
void WDC65C02::w65c02::LSR()
{
	setFlag(C, opd & 0x01);
	temp.result_8 = opd >> 1;
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(N, temp.result_8 & 0x80);

	if (opsLUT[IR].fetch == &w65c02::ACC)
	{
		A = temp.result_8;
	}
	else
	{
		bus->write(fetchedAdr, temp.result_8);
	}
}
// No Operation
void WDC65C02::w65c02::NOP()
{
	return;
}
// Logical OR
void WDC65C02::w65c02::ORA()
{
	A |= opd;
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
}
// Push Accumulator
void WDC65C02::w65c02::PHA()
{
	stackPush(A);
}
// Push Status Register
void WDC65C02::w65c02::PHP()
{
	stackPush(SR | B | _);
}
// Pull Accumulator
void WDC65C02::w65c02::PLA()
{
	A = stackPop();
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
}
// Pull Status Register
void WDC65C02::w65c02::PLP()
{
	SR = stackPop();
	setFlag(_, 1);	// unused flag is always set
	setFlag(B, 0);
}
// Rotate left
void WDC65C02::w65c02::ROL()
{
	temp.result_16 = opd << 1;
	temp.result_16 |= getFlag(C);
	//x temp.result_8 = temp.result_16 & 0x00FF;

	setFlag(C, temp.result_16 & 0xFF00);
	setFlag(N, temp.result_8  & 0x0080);
	setFlag(Z, temp.result_8 == 0x00);

	if (opsLUT[IR].fetch == &w65c02::ACC)
	{
		A = temp.result_8;
	}
	else
	{
		bus->write(fetchedAdr, temp.result_8);
	}
}
// Rotate Right
void WDC65C02::w65c02::ROR()
{
	temp.result_8 = opd >> 1;
	temp.result_8 |= getFlag(C) << 7;

	setFlag(C, opd & 0x01);
	setFlag(N, temp.result_8 & 0x80);
	setFlag(Z, temp.result_8 == 0x00);

	if (opsLUT[IR].fetch == &w65c02::ACC)
	{
		A = temp.result_8;
	}
	else
	{
		bus->write(fetchedAdr, temp.result_8);
	}
}
// Return from interrupt
void WDC65C02::w65c02::RTI()
{
	SR = stackPop();
	setFlag(B, 0);

	uint8_t pcl = stackPop();
	uint8_t pch = stackPop();
	PC = (pch << 8) | pcl;
}
// Return from subroutine
void WDC65C02::w65c02::RTS()
{
	uint8_t pcl = stackPop();
	uint8_t pch = stackPop();
	PC = (pch << 8) | pcl;
	PC++;
}
// Substract with carry
void WDC65C02::w65c02::SBC()
{
	uint8_t ones_cmp = ~opd;

	uint8_t lo_nibble = (A & 0x0F) + (ones_cmp & 0x0F) + getFlag(C);
	uint8_t hi_nibble = (A >> 4) + (ones_cmp >> 4) + (lo_nibble > 0x0F);
	temp.result_16 = (hi_nibble << 4) | (lo_nibble & 0x0F);
	//x temp.result_8 = temp.result_16 & 0x00FF;

	if (getFlag(D))
	{
		if ((lo_nibble & 0x10) == 0) lo_nibble += 0x0A;
		if ((hi_nibble & 0x10) == 0) hi_nibble += 0x0A;

		temp.result_8 = (hi_nibble << 4) | (lo_nibble & 0x0F);
		cycles++;
	}

	setFlag(C, temp.result_16 & 0xFF00);
	setFlag(N, temp.result_8 & 0x80);
	setFlag(Z, temp.result_8 == 0x00);
	setFlag(V, (~(A ^ opd) & (A ^ temp.result_8)) & 0x80);

	A = temp.result_8;
}
// Set carry
void WDC65C02::w65c02::SEC()
{
	setFlag(C, 1);
}
// Set decimal
void WDC65C02::w65c02::SED()
{
	setFlag(D, 1);
}
// Set interrupt
void WDC65C02::w65c02::SEI()
{
	setFlag(I, 1);
}
// Store Accumulator
void WDC65C02::w65c02::STA()
{
	bus->write(fetchedAdr, A);
}
// Store index X
void WDC65C02::w65c02::STX()
{
	bus->write(fetchedAdr, X);
}
// Store index Y
void WDC65C02::w65c02::STY()
{
	bus->write(fetchedAdr, Y);
}
// Transfer A to X
void WDC65C02::w65c02::TAX()
{
	X = A;
	setFlag(Z, X == 0x00);
	setFlag(N, X & 0x80);
}
// Transfer A to Y
void WDC65C02::w65c02::TAY()
{
	Y = A;
	setFlag(Z, Y == 0x00);
	setFlag(N, Y & 0x80);
}
// Transfer Stack Pointer to X
void WDC65C02::w65c02::TSX()
{
	X = SP;
	setFlag(Z, X == 0x00);
	setFlag(N, X & 0x80);
}
// Transfer X to A
void WDC65C02::w65c02::TXA()
{
	A = X;
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
}
// Transfer X to Stack Pointer
void WDC65C02::w65c02::TXS()
{
	SP = X;
}
// Transfer Y to A
void WDC65C02::w65c02::TYA()
{
	A = Y;
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
}

// ===== 65c02 Instructions =====

// Branch on bit 0 reset
void WDC65C02::w65c02::BBR0()
{
	BranchIf(0, false);
}
// Branch on bit 1 reset
void WDC65C02::w65c02::BBR1()
{
	BranchIf(1, false);
}
// Branch on bit 2 reset
void WDC65C02::w65c02::BBR2()
{
	BranchIf(2, false);
}
// Branch on bit 3 reset
void WDC65C02::w65c02::BBR3()
{
	BranchIf(3, false);
}
// Branch on bit 4 reset
void WDC65C02::w65c02::BBR4()
{
	BranchIf(4, false);
}
// Branch on bit 5 reset
void WDC65C02::w65c02::BBR5()
{
	BranchIf(5, false);
}
// Branch on bit 6 reset
void WDC65C02::w65c02::BBR6()
{
	BranchIf(6, false);
}
// Branch on bit 7 reset
void WDC65C02::w65c02::BBR7()
{
	BranchIf(7, false);
}
// Branch on bit 0 set
void WDC65C02::w65c02::BBS0()
{
	BranchIf(0, true);
}
// Branch on bit 1 set
void WDC65C02::w65c02::BBS1()
{
	BranchIf(1, true);
}
// Branch on bit 2 set
void WDC65C02::w65c02::BBS2()
{
	BranchIf(2, true);
}
// Branch on bit 3 set
void WDC65C02::w65c02::BBS3()
{
	BranchIf(3, true);
}
// Branch on bit 4 set
void WDC65C02::w65c02::BBS4()
{
	BranchIf(4, true);
}
// Branch on bit 5 set
void WDC65C02::w65c02::BBS5()
{
	BranchIf(5, true);
}
// Branch on bit 6 set
void WDC65C02::w65c02::BBS6()
{
	BranchIf(6, true);
}
// Branch on bit 7 set
void WDC65C02::w65c02::BBS7()
{
	BranchIf(7, true);
}
// Branch always
void WDC65C02::w65c02::BRA()
{
	if ((PC & 0xFF00) != (fetchedAdr & 0xFF00)) cycles++;
	PC = fetchedAdr;
}
// Push X
void WDC65C02::w65c02::PHX()
{
	stackPush(X);
}
// Push Y
void WDC65C02::w65c02::PHY()
{
	stackPush(Y);
}
// Pull X
void WDC65C02::w65c02::PLX()
{
	X = stackPop();
	setFlag(Z, X == 0x00);
	setFlag(N, X & 0x80);
}
// Pull Y
void WDC65C02::w65c02::PLY()
{
	Y = stackPop();
	setFlag(Z, Y == 0x00);
	setFlag(N, Y & 0x80);
}
// Reset bit 0
void WDC65C02::w65c02::RMB0()
{
	bus->write(fetchedAdr, opd & ~(1 << 0));
}
// Reset bit 1
void WDC65C02::w65c02::RMB1()
{
	bus->write(fetchedAdr, opd & ~(1 << 1));
}
// Reset bit 2
void WDC65C02::w65c02::RMB2()
{
	bus->write(fetchedAdr, opd & ~(1 << 2));
}
// Reset bit 3
void WDC65C02::w65c02::RMB3()
{
	bus->write(fetchedAdr, opd & ~(1 << 3));
}
// Reset bit 4
void WDC65C02::w65c02::RMB4()
{
	bus->write(fetchedAdr, opd & ~(1 << 4));
}
// Reset bit 5
void WDC65C02::w65c02::RMB5()
{
	bus->write(fetchedAdr, opd & ~(1 << 5));
}
// Reset bit 6
void WDC65C02::w65c02::RMB6()
{
	bus->write(fetchedAdr, opd & ~(1 << 6));
}
// Reset bit 7
void WDC65C02::w65c02::RMB7()
{
	bus->write(fetchedAdr, opd & ~(1 << 7));
}
// Set bit 0
void WDC65C02::w65c02::SMB0()
{
	bus->write(fetchedAdr, opd | (1 << 0));
}
// Set bit 1
void WDC65C02::w65c02::SMB1()
{
	bus->write(fetchedAdr, opd | (1 << 1));
}
// Set bit 2
void WDC65C02::w65c02::SMB2()
{
	bus->write(fetchedAdr, opd | (1 << 2));
}
// Set bit 3
void WDC65C02::w65c02::SMB3()
{
	bus->write(fetchedAdr, opd | (1 << 3));
}
// Set bit 4
void WDC65C02::w65c02::SMB4()
{
	bus->write(fetchedAdr, opd | (1 << 4));
}
// Set bit 5
void WDC65C02::w65c02::SMB5()
{
	bus->write(fetchedAdr, opd | (1 << 5));
}
// Set bit 6
void WDC65C02::w65c02::SMB6()
{
	bus->write(fetchedAdr, opd | (1 << 6));
}
// Set bit 7
void WDC65C02::w65c02::SMB7()
{
	bus->write(fetchedAdr, opd | (1 << 7));
}
// Stop
void WDC65C02::w65c02::STP()
{
	stop = true;
}
// Store Zero
void WDC65C02::w65c02::STZ()
{
	bus->write(fetchedAdr, 0x00);
}
// Test and Reset (clear) Bits
void WDC65C02::w65c02::TRB()
{
	setFlag(Z, (A & opd) == 0);

	bus->write(fetchedAdr, opd & ~A);
}
// Test and Set Bits
void WDC65C02::w65c02::TSB()
{
	setFlag(Z, (A & opd) == 0);

	bus->write(fetchedAdr, opd | A);
}
// Wait for interrupt
void WDC65C02::w65c02::WAI()
{
	wait = true;
}