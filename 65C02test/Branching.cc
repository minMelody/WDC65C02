#include <gtest/gtest.h>
#include <vector>

#include "Bus.h"
#include "Test65c02.hpp"
#include "FakeMemory.hpp"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct TestBranch : public testing::Test
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

		TEST_F(TestBranch, JMPAbsoluteIndirectX)
		{
			cpu.X = 0x10;
			mem.data[cpu.PC]   = 0x30;
			mem.data[cpu.PC+1] = 0x02;
			mem.data[0x0240]   = 0xaa;
			mem.data[0x0241]   = 0xee;

			cpu.given(0, cpu.fetchAddress(0x7C));
			cpu.testInstruction(0x7C);

			EXPECT_EQ(cpu.cycles, 6);
			EXPECT_EQ(cpu.PC, 0xeeaa);
		}

		TEST_F(TestBranch, BBR)
		{
			std::vector<uint8_t> opcodes =
			{
				0x0F,
				0x1F,
				0x2F,
				0x3F,
				0x4F,
				0x5F,
				0x6F,
				0x7F
			};

			for (int i = 0; i < opcodes.size(); i++)
			{
				uint16_t prevState = std::rand() & 0xFFFF;
				cpu.PC = prevState;
				mem.data[0x000f] = 0x0a;
				mem.data[cpu.PC] = 0x0f;
				int8_t offset = std::rand() & 0xFF;
				mem.data[cpu.PC + 1] = offset;

				bool branch_taken = (mem.data[0x000f] & (1 << i)) == 0;
				uint16_t expected = prevState + 2 + (offset * branch_taken);
				bool boundary_cross = (cpu.PC & 0xFF00) != (expected & 0xFF00);

				cpu.given(cpu.fetchAddress(opcodes[i]));
				cpu.testInstruction(opcodes[i]);
				//printf("$%02X: PC:$%04X  expected:$%04X = $%04X + ($%02X * %d) + %d\n", opcodes[i], cpu.PC, expected, prevState+2, (uint8_t)offset, branch_taken, boundary_cross);

				EXPECT_EQ(cpu.cycles, 5 + branch_taken + boundary_cross);
				EXPECT_EQ(cpu.PC, expected);
			}
		}
		TEST_F(TestBranch, BBS)
		{
			std::vector<uint8_t> opcodes =
			{
				0x8F,
				0x9F,
				0xAF,
				0xBF,
				0xCF,
				0xDF,
				0xEF,
				0xFF
			};

			for (int i = 0; i < opcodes.size(); i++)
			{
				uint16_t prevState = std::rand() & 0xFFFF;
				cpu.PC = prevState;
				mem.data[0x000f] = 0x0a;
				mem.data[cpu.PC] = 0x0f;
				int8_t offset = std::rand() & 0xFF;
				mem.data[cpu.PC + 1] = offset;

				bool branch_taken = (mem.data[0x000f] & (1 << i)) > 0;
				uint16_t expected = prevState + 2 + (offset * branch_taken);
				bool boundary_cross = (cpu.PC & 0xFF00) != (expected & 0xFF00);

				cpu.given(cpu.fetchAddress(opcodes[i]));
				cpu.testInstruction(opcodes[i]);
				//printf("$%02X: PC:$%04X  expected:$%04X = $%04X + ($%02X * %d) + %d\n", opcodes[i], cpu.PC, expected, prevState + 2, (uint8_t)offset, branch_taken, boundary_cross);

				EXPECT_EQ(cpu.cycles, 5 + branch_taken + boundary_cross);
				EXPECT_EQ(cpu.PC, expected);
			}
		}

		TEST_F(TestBranch, BRA)
		{
			uint16_t prevState = cpu.PC;
			mem.data[cpu.PC] = 0x04;

			cpu.given(0, cpu.fetchAddress(0x80));
			cpu.testInstruction(0x80);

			EXPECT_EQ(cpu.cycles, 3);
			EXPECT_EQ(cpu.PC, prevState + 5);
		}
	}
}