#pragma once

#include <cstdint>
#include <vector>

namespace WDC65C02
{
	struct Bus;

	class w65c02
	{
	public:
		w65c02(Bus* bus);
		~w65c02();

		// CPU internal registers
		uint16_t PC;	// Program Counter
		uint8_t  IR;	// Instruction Register
		uint8_t  A;		// Accumulator Register
		uint8_t  X;		// Index Register X
		uint8_t  Y;		// Index Register Y
		uint8_t  SP;	// Stack Pointer

		// Processor Status Register
		uint8_t SR;
		enum Statusflags
		{
			C = 0b00000001,	// Carry Bit Flag
			Z = 0b00000010,	// Zero Flag
			I = 0b00000100,	// Interrrupt Disable Flag
			D = 0b00001000,	// Decimal Mode Flag
			B = 0b00010000,	// Break Flag
			_ = 0b00100000,	// Unused Flag
			V = 0b01000000,	// Overflow Flag
			N = 0b10000000	// Negative Flag
		};

		bool getFlag(Statusflags f);
		void setFlag(Statusflags f, bool v);
	public:
		void reset();
		// Indicates a wait or stop condition when false
		bool ready();
		// Indicates an instruction has been completed when true,
		// useful for single instruction stepping
		bool sync();
		bool irqPending, nmiPending;

		// Steps one clock cycle
		void step();

	protected:
		bool stop, wait;
		void irq();	// Handle Interrupt Request
		void nmi();	// handle Non-Maskable Interrupt

		Bus* bus = nullptr;

		uint16_t
			ACC(bool ec), IMM(bool ec), IMP(bool ec),
			ABS(bool ec), ABX(bool ec), ABY(bool ec), IND(bool ec), REL(bool ec),
			ZIX(bool ec), ZIY(bool ec), ZP(bool ec), ZPX(bool ec), ZPY(bool ec),
			// 65c02 addressing modes
			AIX(bool ec), ZPI(bool ec), ZPR(bool ec);

		void
			ADC(), AND(), ASL(),
			BCC(), BCS(), BEQ(), BIT(), BMI(), BNE(), BPL(), BRK(), BVC(), BVS(),
			CLC(), CLD(), CLI(), CLV(), CMP(), CPX(), CPY(),
			DEC(), DEX(), DEY(),
			EOR(),
			INC(), INX(), INY(),
			JMP(), JSR(),
			LDA(), LDX(), LDY(), LSR(),
			NOP(),
			ORA(),
			PHA(), PHP(), PLA(), PLP(),
			ROL(), ROR(), RTI(), RTS(),
			SBC(), SEC(), SED(), SEI(), STA(), STX(), STY(),
			TAX(), TAY(), TSX(), TXA(), TXS(), TYA();

		// 65c02 instructions
		void
			BBR0(), BBR1(), BBR2(), BBR3(), BBR4(), BBR5(), BBR6(), BBR7(),
			BBS0(), BBS1(), BBS2(), BBS3(), BBS4(), BBS5(), BBS6(), BBS7(),
			BRA(),
			PHX(), PHY(), PLX(), PLY(),
			RMB0(), RMB1(), RMB2(), RMB3(), RMB4(), RMB5(), RMB6(), RMB7(),
			SMB0(), SMB1(), SMB2(), SMB3(), SMB4(), SMB5(), SMB6(), SMB7(),
			STP(), STZ(),
			TRB(), TSB(),
			WAI();

		struct Instruction
		{
			void (w65c02::* execute)() = nullptr;
			uint16_t(w65c02::* fetch)(bool) = nullptr;
			uint8_t cycles = 0;
			bool extraCycle = false; // Some instructions require one extra cycle if crossing a page boundary when fetching the address
		};
		std::vector<Instruction> opsLUT;

		uint8_t cycles = 0;
		uint8_t opd;
		uint16_t fetchedAdr;

		union
		{
			uint8_t result_8;
			uint16_t result_16;
		} temp;

		uint8_t stackPop();
		void stackPush(uint8_t byte);
		void BranchIf(Statusflags f, bool condition);
		void BranchIf(uint8_t bit, bool condition);
	};
}