#pragma once

#include "w65c02.h"

namespace WDC65C02
{
	namespace UnitTests
	{
		// Expose protected members and methods to the test environment
		class Test65C02 : public w65c02
		{
		public:
			using w65c02::w65c02;	// using constructor

			using w65c02::cycles;
			using w65c02::stackPush;
			using w65c02::stackPop;
			using w65c02::irq;
			using w65c02::nmi;

			Instruction getIns() { return opsLUT[IR]; }

			// Set operand and operand address for test
			void given(uint8_t value, uint16_t address)
			{
				opd = value;
				fetchedAdr = address;
			}
			void given(uint8_t value)
			{
				opd = value;
			}

			uint16_t fetchAddress(uint8_t opcode) { return (this->*opsLUT[opcode].fetch)(opsLUT[opcode].extraCycle); }

			// Execute an instruction by opcode
			void testInstruction(uint8_t opcode)
			{
				IR = opcode;

				cycles = opsLUT[IR].cycles;
				setFlag(_, 1); // unused flag is always set

				(this->*opsLUT[IR].execute)();
			}
		};
	}
}
