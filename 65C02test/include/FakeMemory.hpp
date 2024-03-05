#pragma once

#include <vector>
#include "IAddressable.h"

namespace WDC65C02
{
	namespace UnitTests
	{
		struct FakeMemory : IAddressable
		{
		public:
			FakeMemory(unsigned int size);
			~FakeMemory();
			std::vector<uint8_t> data;
			uint16_t size() const;
			uint8_t read(uint16_t adr);
			uint8_t readc(uint16_t adr) const;
			void write(uint16_t adr, uint8_t byte);
		};
	}
}
