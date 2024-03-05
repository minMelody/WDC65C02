#include <gtest/gtest.h>

#include "Test65c02.hpp"
#include "Bus.h"
#include "FakeMemory.hpp"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct TestProgram : testing::Test
		{
			Bus bus;
			FakeMemory mem{ 0x10000 };
			w65c02 cpu{ &bus };

			virtual void SetUp()
			{
				bus.connect(&mem, 0x0000);
				cpu.reset();
			}
			virtual void TearDown()
			{
			}
		};

		TEST_F(TestProgram, Multiply)
		{
			mem.data[0xFFFC] = 0x00;
			mem.data[0xFFFD] = 0x80;

			mem.data[0x0000] = 3;
			mem.data[0x0001] = 7;
			uint8_t expected = 21;

			mem.data[0x8000] = 0xa6; // LDX $00
			mem.data[0x8001] = 0x00;
			mem.data[0x8002] = 0x65; // ADC $01
			mem.data[0x8003] = 0x01;
			mem.data[0x8004] = 0xca; // DEX
			mem.data[0x8005] = 0xd0; // BNE
			mem.data[0x8006] = 0xfb; // -5
			mem.data[0x8007] = 0xdb; // STP

			cpu.reset();

			std::cout << "Zp:";
			for (uint16_t adr = 0x00; adr < 0x10; adr++)
			{
				printf(" %02X", mem.data[adr]);
			}
			std::cout << std::endl;

			while (cpu.ready())
			{
				cpu.step();
			}

			EXPECT_EQ(cpu.A, expected);
		}
	}
}