#include <gtest/gtest.h>
#include <vector>

#include "Bus.h"
#include "Test65c02.hpp"
#include "FakeMemory.hpp"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct TestNOPs : public testing::Test
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

		TEST_F(TestNOPs, x2)
		{
			std::vector<uint8_t> opcodes =
			{
				0x02,
				0x22,
				0x42,
				0x62,
				0x82,
				0xc2,
				0xe2
			};

			for (int i = 0; i < opcodes.size(); i++)
			{
				cpu.PC = 0;
				cpu.fetchAddress(opcodes[i]);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, 2);
				EXPECT_EQ(cpu.PC, 1);
			}
		}
		TEST_F(TestNOPs, x3)
		{
			std::vector<uint8_t> opcodes =
			{
				0x03,
				0x13,
				0x23,
				0x33,
				0x43,
				0x53,
				0x63,
				0x73,
				0x83,
				0xa3,
				0xb3,
				0xc3,
				0xd3,
				0xe3,
				0xf3
			};

			for (int i = 0; i < opcodes.size(); i++)
			{
				cpu.PC = 0;
				cpu.fetchAddress(opcodes[i]);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, 1);
				EXPECT_EQ(cpu.PC, 0);
			}
		}
		TEST_F(TestNOPs, x4)
		{
			std::vector<uint8_t> opcodes =
			{
				0x44, 3,
				0x54, 4,
				0xd4, 4,
				0xf4, 4
			};

			for (int i = 0; i < opcodes.size() - 1; i += 2)
			{
				cpu.PC = 0;
				cpu.fetchAddress(opcodes[i]);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, opcodes[i+1]);
				EXPECT_EQ(cpu.PC, 1);
			}
		}
		TEST_F(TestNOPs, xb)
		{
			std::vector<uint8_t> opcodes =
			{
				0x0b,
				0x1b,
				0x2b,
				0x3b,
				0x4b,
				0x5b,
				0x6b,
				0x7b,
				0x8b,
				0xab,
				0xbb,
				0xeb,
				0xfb
			};

			for (int i = 0; i < opcodes.size(); i++)
			{
				cpu.PC = 0;
				cpu.fetchAddress(opcodes[i]);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, 1);
				EXPECT_EQ(cpu.PC, 0);
			}
		}
		TEST_F(TestNOPs, xc)
		{
			std::vector<uint8_t> opcodes =
			{
				0x5c, 8,
				0xdc, 4,
				0xfc, 4
			};

			for (int i = 0; i < opcodes.size() - 1; i += 2)
			{
				cpu.PC = 0;
				cpu.fetchAddress(opcodes[i]);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, opcodes[i + 1]);
				EXPECT_EQ(cpu.PC, 2);
			}
		}
	}
}