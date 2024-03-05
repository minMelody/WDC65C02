#include <gtest/gtest.h>
#include <vector>

#include "Bus.h"
#include "Test65c02.hpp"
#include "FakeMemory.hpp"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct SetResetBits : public testing::Test
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

		TEST_F(SetResetBits, RMB)
		{
			std::vector<uint8_t> opcodes =
			{
				0x07,
				0x17,
				0x27,
				0x37,
				0x47,
				0x57,
				0x67,
				0x77
			};

			for (int i = 0; i < opcodes.size(); i++)
			{
				uint8_t v = std::rand() & 0xFF;

				cpu.given(v, 0x0f);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, 5);
				EXPECT_EQ(mem.data[0x000f], v & ~(1 << i));
			}
		}
		TEST_F(SetResetBits, SMB)
		{
			std::vector<uint8_t> opcodes =
			{
				0x87,
				0x97,
				0xA7,
				0xB7,
				0xC7,
				0xD7,
				0xE7,
				0xF7
			};

			for (int i = 0; i < opcodes.size(); i++)
			{
				uint8_t v = std::rand() & 0xFF;

				cpu.given(v, 0x0f);
				cpu.testInstruction(opcodes[i]);

				EXPECT_EQ(cpu.cycles, 5);
				EXPECT_EQ(mem.data[0x000f], v | (1 << i));
			}
		}
	}
}