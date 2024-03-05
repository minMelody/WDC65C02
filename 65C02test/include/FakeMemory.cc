#include <iterator>
#include "FakeMemory.hpp"

WDC65C02::UnitTests::FakeMemory::FakeMemory(unsigned int size)
{
    data.resize(size);
    std::fill(std::begin(data), std::end(data), 0x00);
}

WDC65C02::UnitTests::FakeMemory::~FakeMemory()
{
}

uint16_t WDC65C02::UnitTests::FakeMemory::size() const
{
    return data.size();
}

uint8_t WDC65C02::UnitTests::FakeMemory::read(uint16_t adr)
{
    return data[adr];
}
uint8_t WDC65C02::UnitTests::FakeMemory::readc(uint16_t adr) const
{
    return data[adr];
}

void WDC65C02::UnitTests::FakeMemory::write(uint16_t adr, uint8_t byte)
{
    data[adr] = byte;
}