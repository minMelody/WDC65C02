// Arithmetics and Logic operations
#include <gtest/gtest.h>
#include <vector>

#include "Bus.h"
#include "Test65c02.hpp"
#include "FakeMemory.hpp"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct TestALU : public testing::Test
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

		// ===== Logic =====

		TEST_F(TestALU, ORAZeroPageIndirect)
		{
			cpu.A = 0x0A;
			cpu.given(0x03);
			cpu.testInstruction(0x12);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_EQ(cpu.A, 0x0B);
		}
		TEST_F(TestALU, ANDZeroPageIndirect)
		{
			cpu.A = 0x0A;
			cpu.given(0x03);
			cpu.testInstruction(0x32);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_EQ(cpu.A, 0x02);
		}
		TEST_F(TestALU, EORZeroPageIndirect)
		{
			cpu.A = 0x0A;
			cpu.given(0x03);
			cpu.testInstruction(0x52);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_EQ(cpu.A, 0x09);
		}

		TEST_F(TestALU, BITNewAddressingModes)
		{
			std::vector<uint8_t> opcodes =
			{
				0x89, 2,
				0x34, 4,
				0x3C, 4 + 1
			};

			for (int i = 0; i < opcodes.size() - 1; i += 2)
			{
				uint8_t a = std::rand() & 0xFF;
				uint8_t b = std::rand() & 0xFF;
				cpu.A = a;
				cpu.given(b, 0x00F0);

				cpu.testInstruction(opcodes[i]);
				//printf("%02X: %02X & %02X\n", opcodes[i], a, b);

				EXPECT_EQ(cpu.cycles + cpu.getIns().extraCycle, opcodes[i + 1]);
				EXPECT_EQ(cpu.getFlag(cpu.Z), (a & b) == 0);
				EXPECT_EQ(cpu.getFlag(cpu.V), ((a & b) & (1 << 6)) > 0);
				EXPECT_EQ(cpu.getFlag(cpu.N), ((a & b) & (1 << 7)) > 0);
			}
		}

		TEST_F(TestALU, TRB)
		{
			std::vector<uint8_t> opcodes =
			{
				0x14, 5,
				0x1C, 6
			};

			for (int i = 0; i < opcodes.size()-1; i+=2)
			{
				uint8_t a = std::rand() & 0xFF;
				uint8_t b = std::rand() & 0xFf;
				cpu.A = a;
				uint8_t expected = b & ~a;

				cpu.given(b, 0x00f0);
				cpu.testInstruction(opcodes[i]);
				//printf("%02X: %02X & ~%02X = %02X\n", opcodes[i], b, a, expected);

				EXPECT_EQ(cpu.cycles, opcodes[i + 1]);
				EXPECT_EQ(cpu.getFlag(cpu.Z), (a & b) == 0);
				EXPECT_EQ(mem.data[0x00f0], expected);
			}
		}
		TEST_F(TestALU, TSB)
		{
			std::vector<uint8_t> opcodes =
			{
				0x04, 5,
				0x0C, 6
			};

			for (int i = 0; i < opcodes.size() - 1; i += 2)
			{
				uint8_t a = std::rand() & 0xFF;
				uint8_t b = std::rand() & 0xFf;
				cpu.A = a;
				uint8_t expected = b | a;

				cpu.given(b, 0x00f0);
				cpu.testInstruction(opcodes[i]);
				//printf("%02X: %02X | %02X = %02X\n", opcodes[i], b, a, expected);

				EXPECT_EQ(cpu.cycles, opcodes[i + 1]);
				EXPECT_EQ(cpu.getFlag(cpu.Z), (a & b) == 0);
				EXPECT_EQ(mem.data[0x00f0], expected);
			}
		}

		// ===== Arithmetics =====

		TEST_F(TestALU, ADCZeroPageIndirect)
		{
			cpu.A = 5;
			cpu.given(4);
			cpu.testInstruction(0x72);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_EQ(cpu.A, 9);
			EXPECT_FALSE(cpu.getFlag(cpu.C));
			EXPECT_FALSE(cpu.getFlag(cpu.N));
			EXPECT_FALSE(cpu.getFlag(cpu.Z));
			EXPECT_FALSE(cpu.getFlag(cpu.V));
		}
		TEST_F(TestALU, SBCZeroPageIndirect)
		{
			cpu.A = 5;
			cpu.setFlag(cpu.C, 1);
			cpu.given(5);
			cpu.testInstruction(0xF2);

			EXPECT_EQ(cpu.cycles, 5);
			EXPECT_EQ(cpu.A, 0);
			EXPECT_TRUE(cpu.getFlag(cpu.C));
			EXPECT_FALSE(cpu.getFlag(cpu.N));
			EXPECT_TRUE (cpu.getFlag(cpu.Z));
			EXPECT_FALSE(cpu.getFlag(cpu.V));
		}

		TEST_F(TestALU, ADCDecimal)
		{
			cpu.setFlag(cpu.D, 1);
			for (int i = 0; i < 100; i++)
			{
				uint8_t a = ((std::rand() % 10) << 4) | (std::rand() % 10);
				uint8_t b = ((std::rand() % 10) << 4) | (std::rand() % 10);
				cpu.A = a;
				cpu.given(b);
				uint8_t expected = (a >> 4) * 10 + (a & 0x0F);
				expected += (b >> 4) * 10 + (b & 0x0F);
				uint8_t expected_8 = expected % 100;

				cpu.setFlag(cpu.C, 0);
				cpu.testInstruction(0x72);
				/*printf("%02X+%02X=%d => $%02X %c%c%c%c\n", a, b, expected, cpu.A,
					cpu.getFlag(cpu.C) ? 'C' : ' ', cpu.getFlag(cpu.N) ? 'N' : ' ',
					cpu.getFlag(cpu.Z) ? 'Z' : ' ', cpu.getFlag(cpu.V) ? 'V' : ' ');*/

				EXPECT_EQ(cpu.cycles, 6);
				uint8_t result = (cpu.A >> 4) * 10 + (cpu.A & 0x0F);
				EXPECT_EQ(result, expected_8);
				EXPECT_EQ(cpu.getFlag(cpu.C), expected > 99);
				EXPECT_EQ(cpu.getFlag(cpu.N), expected_8 >= 80);
				EXPECT_EQ(cpu.getFlag(cpu.Z), expected_8 == 0x00);
			}
		}
		TEST_F(TestALU, ADCDecimalWithZero)
		{
			cpu.setFlag(cpu.D, 1);

			cpu.A = 0;
			cpu.given(0);

			cpu.testInstruction(0x72);

			EXPECT_EQ(cpu.cycles, 6);
			EXPECT_EQ(cpu.A, 0);
			EXPECT_FALSE(cpu.getFlag(cpu.C));
			EXPECT_FALSE(cpu.getFlag(cpu.N));
			EXPECT_TRUE(cpu.getFlag(cpu.Z));
		}

		TEST_F(TestALU, SBCDecimal)
		{
			cpu.setFlag(cpu.D, 1);
			for (int i = 0; i < 100; i++)
			{
				uint8_t a = ((std::rand() % 10) << 4) | (std::rand() % 10);
				uint8_t b = ((std::rand() % 10) << 4) | (std::rand() % 10);
				cpu.A = a;
				cpu.given(b);
				int8_t expected = (a >> 4) * 10 + (a & 0x0F);
				expected -= (b >> 4) * 10 + (b & 0x0F);

				cpu.setFlag(cpu.C, 1);
				cpu.testInstruction(0xF2);
				/*printf("%02X-%02X=%d => $%02X %c%c%c%c\n", a, b, expected, cpu.A,
					cpu.getFlag(cpu.C) ? 'C' : ' ', cpu.getFlag(cpu.N) ? 'N' : ' ',
					cpu.getFlag(cpu.Z) ? 'Z' : ' ', cpu.getFlag(cpu.V) ? 'V' : ' ');*/

				if (expected < 0) expected = 100 + expected;
				uint8_t expected_8 = expected % 100;

				EXPECT_EQ(cpu.cycles, 6);
				uint8_t result = (cpu.A >> 4) * 10 + (cpu.A & 0x0F);
				EXPECT_EQ(result, expected_8);
				EXPECT_EQ(cpu.getFlag(cpu.C), a >= b);
				EXPECT_EQ(cpu.getFlag(cpu.N), expected_8 >= 80);
				EXPECT_EQ(cpu.getFlag(cpu.Z), expected_8 == 0x00);
			}
		}
		TEST_F(TestALU, SBCDecimalWithZero)
		{
			cpu.setFlag(cpu.D, 1);
			cpu.setFlag(cpu.C, 1);

			cpu.A = 5;
			cpu.given(5);

			cpu.testInstruction(0xF2);

			EXPECT_EQ(cpu.cycles, 6);
			EXPECT_EQ(cpu.A, 0);
			EXPECT_TRUE(cpu.getFlag(cpu.C));
			EXPECT_FALSE(cpu.getFlag(cpu.N));
			EXPECT_TRUE(cpu.getFlag(cpu.Z));
		}
	}
}