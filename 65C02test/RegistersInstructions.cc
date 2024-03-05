#include <gtest/gtest.h>
#include <vector>

#include "Bus.h"
#include "Test65c02.hpp"
#include "FakeMemory.hpp"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct RegIns : public testing::Test
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

		TEST_F(RegIns, STZ)
		{
			std::vector<uint8_t> opcodes =
			{
				0x64, 3,
				0x74, 4,
				0x9C, 4,
				0x9E, 5
			};

			for (int i = 0; i < opcodes.size() - 1; i += 2)
			{
				mem.data[0x000f] = 0xea;

				cpu.given(0, 0x000f);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, opcodes[i + 1]);
				EXPECT_EQ(mem.data[0x000f], 0x00);
			}
		}

		TEST_F(RegIns, STAZeroPageIndirect)
		{
			cpu.A = 0x50;
			mem.data[cpu.PC] = 0x20;
			mem.data[0x0020] = 0x31;
			mem.data[0x0021] = 0x65;

			cpu.given(0, cpu.fetchAddress(0xB2));
			cpu.testInstruction(0x92);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_EQ(mem.data[0x6531], cpu.A);
		}
		TEST_F(RegIns, LDAZeroPageIndirect)
		{
			cpu.A = 0x50;
			mem.data[cpu.PC] = 0x20;
			mem.data[0x0020] = 0x31;
			mem.data[0x0021] = 0x65;
			mem.data[0x6531] = 0xea;

			cpu.given(mem.data[cpu.fetchAddress(0xB2)]);
			cpu.testInstruction(0xB2);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_EQ(cpu.A, mem.data[0x6531]);
		}

		TEST_F(RegIns, CMPZeroPageIndirect)
		{
			cpu.A = 0xf0;
			cpu.given(0x05);

			cpu.testInstruction(0xD2);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_TRUE(cpu.getFlag(cpu.C));
			EXPECT_FALSE(cpu.getFlag(cpu.Z));
			EXPECT_TRUE(cpu.getFlag(cpu.N));
		}

		TEST_F(RegIns, INCAccumulator)
		{
			cpu.A = 5;
			cpu.given(cpu.fetchAddress(0x1A));
			cpu.testInstruction(0x1A);

			EXPECT_EQ(cpu.cycles, 2);
			EXPECT_EQ(cpu.A, 6);
		}
		TEST_F(RegIns, DECAccumulator)
		{
			cpu.A = 5;
			cpu.given(cpu.fetchAddress(0x3A));
			cpu.testInstruction(0x3A);

			EXPECT_EQ(cpu.cycles, 2);
			EXPECT_EQ(cpu.A, 4);
		}

		TEST_F(RegIns, PHX)
		{
			cpu.X = 0xea;
			cpu.testInstruction(0xDA);

			EXPECT_EQ(cpu.cycles, 3);
			EXPECT_EQ(mem.data[0x0100 | cpu.SP + 1], cpu.X);
		}
		TEST_F(RegIns, PHY)
		{
			cpu.Y = 0xea;
			cpu.testInstruction(0x5A);

			EXPECT_EQ(cpu.cycles, 3);
			EXPECT_EQ(mem.data[0x0100 | cpu.SP + 1], cpu.Y);
		}
		TEST_F(RegIns, PLX)
		{
			cpu.stackPush(0xab);
			cpu.testInstruction(0xFA);

			EXPECT_EQ(cpu.cycles, 4);
			EXPECT_EQ(cpu.X, 0xab);
		}
		TEST_F(RegIns, PLY)
		{
			cpu.stackPush(0xab);
			cpu.testInstruction(0x7A);

			EXPECT_EQ(cpu.cycles, 4);
			EXPECT_EQ(cpu.Y, 0xab);
		}
	}
}