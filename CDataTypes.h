#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <math.h>

using namespace std;

typedef uint16_t Address;
typedef uint16_t Byte16Bit;

typedef uint8_t Byte, Opcode;
 ;

typedef int8_t signedByte;
typedef int16_t signedByte16Bit;


const Byte BIT_7 = 7;
const Byte BIT_6 = 6;
const Byte BIT_5 = 5;
const Byte BIT_4 = 4;
const Byte BIT_3 = 3;
const Byte BIT_2 = 2;
const Byte BIT_1 = 1;
const Byte BIT_0 = 0;


//this sets the values of the interrupt registers

const Byte VBlankInterrupt = 0;
const Byte LCDCInterrupt = 1;
const Byte TimerInterrupt = 2;
const Byte SerailInterrupt = 3;
const Byte JoypadInterrupt = 4;


// these are the key ID for the keys that will be used for the joypad of the emulator
const int A = 0;
const int B = 18;
const int START = 25;
const int SELECT = 23;
const int LEFT = 71;
const int RIGHT = 72;
const int UP = 73;
const int DOWN = 74;


Byte highByte(Byte16Bit data);
Byte lowByte(Byte16Bit data);
Byte highNibble(Byte data);
Byte lowNibble(Byte data);
Byte16Bit combineByte(Byte high, Byte low);
bool betweenValues(Byte target, int low, int high);
Byte setBit(Byte data, Byte bit);
Byte clearBit(Byte data, Byte bit);
bool bitSet(Byte data, Byte bit);


class Pair
{
private:
	Byte& high;
	Byte& low;

public:
	Pair(Byte& high_, Byte& low_) : high(high_), low(low_) {}
	void increase();
	void decrease();
	void set(Byte16Bit value);
	void set(Byte upper, Byte lower);
	Byte16Bit get();

	Address address();
};

class MemoryRegister
{
private:
	Byte* value;

public:
	MemoryRegister();
	MemoryRegister(Byte* _data);
	Byte get();
	void set(Byte data);
	void clear();
	void bitSet(Byte bit);
	void bitClear(Byte bit);
	bool bitIsSet(Byte bit);
};

