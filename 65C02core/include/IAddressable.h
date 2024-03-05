#pragma once

#include <cstdint>

namespace WDC65C02
{
	struct IAddressable
	{
	public:
		uint16_t start;
		uint16_t end;
		virtual uint16_t size() const = 0;

		virtual uint8_t read(uint16_t adr) = 0;
		virtual uint8_t readc(uint16_t adr) const = 0;
		virtual void write(uint16_t adr, uint8_t byte) = 0;
	};
}