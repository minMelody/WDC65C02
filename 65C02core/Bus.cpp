#include "Bus.h"
#include <stdexcept>

uint8_t WDC65C02::Bus::read(uint16_t address)
{
	IAddressable* addressable = getAddressable(address);
	return addressable->read(address - addressable->start);
}

void WDC65C02::Bus::write(uint16_t address, uint8_t data)
{
	IAddressable* addressable = getAddressable(address);
	if (addressable != nullptr)
	{
		addressable->write(address - addressable->start, data);
	}
}

// ===== Address Space =====

// Maps some memory structure to the address space.
void WDC65C02::Bus::connect(IAddressable* addressable, uint16_t address)
{
	addressable->start = address;
	addressable->end = address + (addressable->size() - 1);
	addressSpace.push_back(addressable);
}

// Returns the addressable memory structure for the specified address.
WDC65C02::IAddressable* WDC65C02::Bus::getAddressable(uint16_t adr)
{
	for (int i = 0; i < addressSpace.size(); i++)
	{
		if (adr >= addressSpace[i]->start && adr <= addressSpace[i]->end)
		{
			return addressSpace[i];
		}
	}
	throw std::range_error("No addressable memory found.");
}