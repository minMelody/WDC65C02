#include <gtest/gtest.h>

#include "w65c02.h"
#include "FakeMemory.hpp"
#include "Bus.h"

namespace WDC65C02
{
	namespace UnitTests
	{
		// Expose protected members and methods to the test environment
		struct Test65C02 : w65c02
		{
			using w65c02::w65c02;
			using w65c02::cycles;
			using w65c02::wait;
			using w65c02::stop;
			// Execute an instruction by opcode
			void testInstruction(uint8_t opcode)
			{
				IR = opcode;

				cycles = opsLUT[IR].cycles;
				setFlag(_, 1); // unused flag is always set

				(this->*opsLUT[opcode].execute)();
			}
		};

		struct TestCoreFs : public testing::Test
		{
			Bus bus;
			FakeMemory mem{ 0x10000 };
			Test65C02 cpu{ &bus };

			virtual void SetUp()
			{
				bus.connect(&mem, 0x0000);
				cpu.reset();
			}
			virtual void TearDown()
			{
			}
		};

		TEST_F(TestCoreFs, TestReset) // See MOS6502 UnitTests for a more comprehensive reset test
		{
			cpu.reset();

			EXPECT_FALSE(cpu.wait);
			EXPECT_FALSE(cpu.stop);
		}

		TEST_F(TestCoreFs, STP)
		{
			cpu.testInstruction(0xDB);

			EXPECT_EQ(cpu.cycles, 3);
			EXPECT_TRUE(cpu.stop);
		}
		TEST_F(TestCoreFs, WAI)
		{
			cpu.testInstruction(0xCB);

			EXPECT_EQ(cpu.cycles, 3);
			EXPECT_TRUE(cpu.wait);
		}
	}
}