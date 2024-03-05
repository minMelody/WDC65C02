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

			using w65c02::ZP;
			using w65c02::IND;
			using w65c02::AIX;
			using w65c02::ZPI;
			using w65c02::ZPR;

			using w65c02::opd;
			using w65c02::fetchedAdr;
		};

		struct TestAdrModes : public testing::Test
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

		// Making sure using one of the MOS6502 function still works correctly
		TEST_F(TestAdrModes, UsingMOSZeroPage)
		{
			mem.data[cpu.PC] = 0xad;
			EXPECT_EQ(cpu.ZP(false), 0x00ad);
		}

		TEST_F(TestAdrModes, IndirectCrossingBoundaryShouldBeFixed)
		{
			mem.data[cpu.PC]   = 0xFF;
			mem.data[cpu.PC+1] = 0x30;
			mem.data[0x3000]   = 0x40;
			mem.data[0x30FF]   = 0x99;
			mem.data[0x3100]   = 0x50;

			EXPECT_EQ(cpu.IND(false), 0x5099);
		}

		TEST_F(TestAdrModes, AbsoluteIndexedIndirect)
		{
			cpu.X = 3;
			mem.data[cpu.PC]   = 0xae;
			mem.data[cpu.PC+1] = 0xea;
			mem.data[0xeab1]   = 0x30;
			mem.data[0xeab2]   = 0x31;

			EXPECT_EQ(cpu.AIX(false), 0x3130);
		}

		TEST_F(TestAdrModes, ZeroPageIndirect)
		{
			mem.data[cpu.PC] = 0x20;
			mem.data[0x0020] = 0x31;
			mem.data[0x0021] = 0x65;

			EXPECT_EQ(cpu.ZPI(false), 0x6531);
		}

		TEST_F(TestAdrModes, ZeroPageWithRelative)
		{
			uint16_t expected = cpu.PC + 9;
			mem.data[cpu.PC]   = 0x20;
			mem.data[cpu.PC+1] = 0x07;
			mem.data[0x0020]   = 0xaa;

			EXPECT_EQ(cpu.ZPR(false), 0xaa);
			EXPECT_EQ(cpu.fetchedAdr, expected);
		}
	}
}