#include "CDataTypes.h"

Byte highByte(Byte16Bit target)
{
	return (Byte)(target >> 8) & 0xFF;
}

Byte lowByte(Byte16Bit target)
{
	return (Byte)target;
}

Byte highNibble(Byte target)
{
	return ((target >> 4) & 0xF);
}

Byte lowNibble(Byte target)
{
	Byte test = (target & 0xF);
	return (target & 0xF);
}

Byte16Bit combineByte(Byte high, Byte low)
{
	return (high << 8 | low);
}

bool betweenValues(Byte target, int low, int high)
{
	return (target >= low && target <= high);
}

Byte setBit(Byte data, Byte bit)
{
	return data | (1 << bit);
}

Byte clearBit(Byte data, Byte bit)
{
	return data & (~(1 << bit));
}

bool bitSet(Byte data, Byte bit)
{
	return ((data >> bit) & 1) ? true : false;
}

void Pair::increase()
{
	Address addr = address() + 1;
	set(addr);
}

void Pair::decrease()
{
	Address addr = address() - 1;
	set(addr);
}

void Pair::set(Byte16Bit value)
{
	high = highByte(value);
	low = lowByte(value);
}

void Pair::set(Byte upper, Byte lower)
{
	high = upper;
	low = lower;
}

Byte16Bit Pair::get()
{
	return address();
}

Address Pair::address()
{
	return combineByte(high, low);
}

MemoryRegister::MemoryRegister() {}

MemoryRegister::MemoryRegister(Byte* _data)
{
	value = _data;
}

Byte MemoryRegister::get()
{
	return *value;
}

void MemoryRegister::set(Byte data)
{
	*value = data;
}

void MemoryRegister::clear()
{
	set(0x00);
}

void MemoryRegister::bitSet(Byte bit)
{
	*value |= 1 << bit;
}

void MemoryRegister::bitClear(Byte bit)
{
	*value &= ~(1 << bit);
}

bool MemoryRegister::bitIsSet(Byte bit)
{
	return ((*value >> bit) & 1) ? true : false;
}