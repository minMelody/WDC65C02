#include <gtest/gtest.h>
#include <vector>

#include "Bus.h"
#include "Test65c02.hpp"
#include "FakeMemory.hpp"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct ChangedCyclecounts : public testing::Test
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

		TEST_F(ChangedCyclecounts, JMPIndirectTakesOneMoreCycle)
		{
			mem.data[cpu.PC] = 0x30;
			mem.data[cpu.PC + 1] = 0x02;
			mem.data[0x0230] = 0xff;
			mem.data[0x0231] = 0xbb;

			cpu.given(0, cpu.fetchAddress(0x6C));
			cpu.testInstruction(0x6C);

			EXPECT_EQ(cpu.cycles, 6);
			EXPECT_EQ(cpu.PC, 0xbbff);
		}

		TEST_F(ChangedCyclecounts, ASLAbosluteX)
		{
			cpu.X = 0x10;
			mem.data[cpu.PC]   = 0x30;
			mem.data[cpu.PC+1] = 0x02;

			cpu.given(0xaa, cpu.fetchAddress(0x1E));
			cpu.testInstruction(0x1E);

			EXPECT_EQ(cpu.cycles + cpu.getIns().extraCycle, 6+1);
			EXPECT_EQ(mem.data[0x0240], 0x54);
			EXPECT_TRUE(cpu.getFlag(cpu.C));
		}
		TEST_F(ChangedCyclecounts, LSRAbosluteX)
		{
			cpu.X = 0x10;
			mem.data[cpu.PC] = 0x30;
			mem.data[cpu.PC + 1] = 0x02;

			cpu.given(0xaa, cpu.fetchAddress(0x5E));
			cpu.testInstruction(0x5E);

			EXPECT_EQ(cpu.cycles + cpu.getIns().extraCycle, 6 + 1);
			EXPECT_EQ(mem.data[0x0240], 0x55);
			EXPECT_FALSE(cpu.getFlag(cpu.C));
		}
		TEST_F(ChangedCyclecounts, ROLAbosluteX)
		{
			cpu.X = 0x10;
			mem.data[cpu.PC] = 0x30;
			mem.data[cpu.PC + 1] = 0x02;

			cpu.setFlag(cpu.C, 1);
			cpu.given(0xaa, cpu.fetchAddress(0x3E));
			cpu.testInstruction(0x3E);

			EXPECT_EQ(cpu.cycles + cpu.getIns().extraCycle, 6 + 1);
			EXPECT_EQ(mem.data[0x0240], 0x55);
			EXPECT_TRUE(cpu.getFlag(cpu.C));
		}
		TEST_F(ChangedCyclecounts, RORAbosluteX)
		{
			cpu.X = 0x10;
			mem.data[cpu.PC] = 0x30;
			mem.data[cpu.PC + 1] = 0x02;

			cpu.setFlag(cpu.C, 1);
			cpu.given(0xaa, cpu.fetchAddress(0x7E));
			cpu.testInstruction(0x7E);

			EXPECT_EQ(cpu.cycles + cpu.getIns().extraCycle, 6 + 1);
			EXPECT_EQ(mem.data[0x0240], 0xD5);
			EXPECT_FALSE(cpu.getFlag(cpu.C));
		}
	}
}