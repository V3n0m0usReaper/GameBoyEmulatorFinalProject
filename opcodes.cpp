#include "CCpu.h"

//these opcodes will only be used when the emulator reads in 0xCB from a ROM allowing for a 
//larger set of commands to be used. Each of the codes are equal to a number and a character that will be read in from the ROM
void CCpu::bitOpParse(Opcode code)
{

	//the switch statement makes it so the correct functions are carried out. each code carries out a different function
	//
	switch (code)
	{
		
	case 0x07: RL(ARegister, false, true); op(2, 2); break;
	case 0x00: RL(BRegister, false, true); op(2, 2); break;
	case 0x01: RL(CRegister, false, true); op(2, 2); break;
	case 0x02: RL(DRegister, false, true); op(2, 2); break;
	case 0x03: RL(ERegister, false, true); op(2, 2); break;
	case 0x04: RL(HRegister, false, true); op(2, 2); break;
	case 0x05: RL(LRegister, false, true); op(2, 2); break;
	case 0x17: RL(ARegister, true, true); op(2, 2); break;
	case 0x10: RL(BRegister, true, true); op(2, 2); break;
	case 0x11: RL(CRegister, true, true); op(2, 2); break;
	case 0x12: RL(DRegister, true, true); op(2, 2); break;
	case 0x13: RL(ERegister, true, true); op(2, 2); break;
	case 0x14: RL(HRegister, true, true); op(2, 2); break;
	case 0x15: RL(LRegister, true, true); op(2, 2); break;

	case 0x06: RL(Pair(HRegister, LRegister).address(), false); op(2, 4); break;
	case 0x16: RL(Pair(HRegister, LRegister).address(), true); op(2, 4); break;

		
	case 0x0F: RR(ARegister, false, true); op(2, 2); break;
	case 0x08: RR(BRegister, false, true); op(2, 2); break;
	case 0x09: RR(CRegister, false, true); op(2, 2); break;
	case 0x0A: RR(DRegister, false, true); op(2, 2); break;
	case 0x0B: RR(ERegister, false, true); op(2, 2); break;
	case 0x0C: RR(HRegister, false, true); op(2, 2); break;
	case 0x0D: RR(LRegister, false, true); op(2, 2); break;
	case 0x1F: RR(ARegister, true, true); op(2, 2); break;
	case 0x18: RR(BRegister, true, true); op(2, 2); break;
	case 0x19: RR(CRegister, true, true); op(2, 2); break;
	case 0x1A: RR(DRegister, true, true); op(2, 2); break;
	case 0x1B: RR(ERegister, true, true); op(2, 2); break;
	case 0x1C: RR(HRegister, true, true); op(2, 2); break;
	case 0x1D: RR(LRegister, true, true); op(2, 2); break;

	case 0x0E: RR(Pair(HRegister, LRegister).address(), false); op(2, 4); break;
	case 0x1E: RR(Pair(HRegister, LRegister).address(), true); op(2, 4); break;


	
	case 0x27: LeftShift(ARegister); op(2, 2); break;
	case 0x20: LeftShift(BRegister); op(2, 2); break;
	case 0x21: LeftShift(CRegister); op(2, 2); break;
	case 0x22: LeftShift(DRegister); op(2, 2); break;
	case 0x23: LeftShift(ERegister); op(2, 2); break;
	case 0x24: LeftShift(HRegister); op(2, 2); break;
	case 0x25: LeftShift(LRegister); op(2, 2); break;
	case 0x26: LeftShift(Pair(HRegister, LRegister).address()); op(2, 4); break;

	case 0x2F: RightShift(ARegister, true); op(2, 2); break;
	case 0x28: RightShift(BRegister, true); op(2, 2); break;
	case 0x29: RightShift(CRegister, true); op(2, 2); break;
	case 0x2A: RightShift(DRegister, true); op(2, 2); break;
	case 0x2B: RightShift(ERegister, true); op(2, 2); break;
	case 0x2C: RightShift(HRegister, true); op(2, 2); break;
	case 0x2D: RightShift(LRegister, true); op(2, 2); break;
	case 0x2E: RightShift(Pair(HRegister, LRegister).address(), true); op(2, 4); break;

	case 0x3F: RightShift(ARegister, false); op(2, 2); break;
	case 0x38: RightShift(BRegister, false); op(2, 2); break;
	case 0x39: RightShift(CRegister, false); op(2, 2); break;
	case 0x3A: RightShift(DRegister, false); op(2, 2); break;
	case 0x3B: RightShift(ERegister, false); op(2, 2); break;
	case 0x3C: RightShift(HRegister, false); op(2, 2); break;
	case 0x3D: RightShift(LRegister, false); op(2, 2); break;
	case 0x3E: RightShift(Pair(HRegister, LRegister).address(), false); op(2, 4); break;

	case 0x37: SWAP(ARegister); op(2, 2); break;
	case 0x30: SWAP(BRegister); op(2, 2); break;
	case 0x31: SWAP(CRegister); op(2, 2); break;
	case 0x32: SWAP(DRegister); op(2, 2); break;
	case 0x33: SWAP(ERegister); op(2, 2); break;
	case 0x34: SWAP(HRegister); op(2, 2); break;
	case 0x35: SWAP(LRegister); op(2, 2); break;
	case 0x36: SWAP(Pair(HRegister, LRegister).address()); op(2, 4); break;

	case 0x47: BIT(ARegister, 0); op(2, 2); break;
	case 0x4F: BIT(ARegister, 1); op(2, 2); break;
	case 0x57: BIT(ARegister, 2); op(2, 2); break;
	case 0x5F: BIT(ARegister, 3); op(2, 2); break;
	case 0x67: BIT(ARegister, 4); op(2, 2); break;
	case 0x6F: BIT(ARegister, 5); op(2, 2); break;
	case 0x77: BIT(ARegister, 6); op(2, 2); break;
	case 0x7F: BIT(ARegister, 7); op(2, 2); break;

	case 0x40: BIT(BRegister, 0); op(2, 2); break;
	case 0x48: BIT(BRegister, 1); op(2, 2); break;
	case 0x50: BIT(BRegister, 2); op(2, 2); break;
	case 0x58: BIT(BRegister, 3); op(2, 2); break;
	case 0x60: BIT(BRegister, 4); op(2, 2); break;
	case 0x68: BIT(BRegister, 5); op(2, 2); break;
	case 0x70: BIT(BRegister, 6); op(2, 2); break;
	case 0x78: BIT(BRegister, 7); op(2, 2); break;

	case 0x41: BIT(CRegister, 0); op(2, 2); break;
	case 0x49: BIT(CRegister, 1); op(2, 2); break;
	case 0x51: BIT(CRegister, 2); op(2, 2); break;
	case 0x59: BIT(CRegister, 3); op(2, 2); break;
	case 0x61: BIT(CRegister, 4); op(2, 2); break;
	case 0x69: BIT(CRegister, 5); op(2, 2); break;
	case 0x71: BIT(CRegister, 6); op(2, 2); break;
	case 0x79: BIT(CRegister, 7); op(2, 2); break;

	case 0x42: BIT(DRegister, 0); op(2, 2); break;
	case 0x4A: BIT(DRegister, 1); op(2, 2); break;
	case 0x52: BIT(DRegister, 2); op(2, 2); break;
	case 0x5A: BIT(DRegister, 3); op(2, 2); break;
	case 0x62: BIT(DRegister, 4); op(2, 2); break;
	case 0x6A: BIT(DRegister, 5); op(2, 2); break;
	case 0x72: BIT(DRegister, 6); op(2, 2); break;
	case 0x7A: BIT(DRegister, 7); op(2, 2); break;

	case 0x43: BIT(ERegister, 0); op(2, 2); break;
	case 0x4B: BIT(ERegister, 1); op(2, 2); break;
	case 0x53: BIT(ERegister, 2); op(2, 2); break;
	case 0x5B: BIT(ERegister, 3); op(2, 2); break;
	case 0x63: BIT(ERegister, 4); op(2, 2); break;
	case 0x6B: BIT(ERegister, 5); op(2, 2); break;
	case 0x73: BIT(ERegister, 6); op(2, 2); break;
	case 0x7B: BIT(ERegister, 7); op(2, 2); break;

	case 0x44: BIT(HRegister, 0); op(2, 2); break;
	case 0x4C: BIT(HRegister, 1); op(2, 2); break;
	case 0x54: BIT(HRegister, 2); op(2, 2); break;
	case 0x5C: BIT(HRegister, 3); op(2, 2); break;
	case 0x64: BIT(HRegister, 4); op(2, 2); break;
	case 0x6C: BIT(HRegister, 5); op(2, 2); break;
	case 0x74: BIT(HRegister, 6); op(2, 2); break;
	case 0x7C: BIT(HRegister, 7); op(2, 2); break;

	case 0x45: BIT(LRegister, 0); op(2, 2); break;
	case 0x4D: BIT(LRegister, 1); op(2, 2); break;
	case 0x55: BIT(LRegister, 2); op(2, 2); break;
	case 0x5D: BIT(LRegister, 3); op(2, 2); break;
	case 0x65: BIT(LRegister, 4); op(2, 2); break;
	case 0x6D: BIT(LRegister, 5); op(2, 2); break;
	case 0x75: BIT(LRegister, 6); op(2, 2); break;
	case 0x7D: BIT(LRegister, 7); op(2, 2); break;

	case 0x46: BIT(Pair(HRegister, LRegister).address(), 0); op(2, 3); break;
	case 0x4E: BIT(Pair(HRegister, LRegister).address(), 1); op(2, 3); break;
	case 0x56: BIT(Pair(HRegister, LRegister).address(), 2); op(2, 3); break;
	case 0x5E: BIT(Pair(HRegister, LRegister).address(), 3); op(2, 3); break;
	case 0x66: BIT(Pair(HRegister, LRegister).address(), 4); op(2, 3); break;
	case 0x6E: BIT(Pair(HRegister, LRegister).address(), 5); op(2, 3); break;
	case 0x76: BIT(Pair(HRegister, LRegister).address(), 6); op(2, 3); break;
	case 0x7E: BIT(Pair(HRegister, LRegister).address(), 7); op(2, 3); break;

	case 0xC7: SET(ARegister, 0); op(2, 2); break;
	case 0xCF: SET(ARegister, 1); op(2, 2); break;
	case 0xD7: SET(ARegister, 2); op(2, 2); break;
	case 0xDF: SET(ARegister, 3); op(2, 2); break;
	case 0xE7: SET(ARegister, 4); op(2, 2); break;
	case 0xEF: SET(ARegister, 5); op(2, 2); break;
	case 0xF7: SET(ARegister, 6); op(2, 2); break;
	case 0xFF: SET(ARegister, 7); op(2, 2); break;

	case 0xC0: SET(BRegister, 0); op(2, 2); break;
	case 0xC8: SET(BRegister, 1); op(2, 2); break;
	case 0xD0: SET(BRegister, 2); op(2, 2); break;
	case 0xD8: SET(BRegister, 3); op(2, 2); break;
	case 0xE0: SET(BRegister, 4); op(2, 2); break;
	case 0xE8: SET(BRegister, 5); op(2, 2); break;
	case 0xF0: SET(BRegister, 6); op(2, 2); break;
	case 0xF8: SET(BRegister, 7); op(2, 2); break;

	case 0xC1: SET(CRegister, 0); op(2, 2); break;
	case 0xC9: SET(CRegister, 1); op(2, 2); break;
	case 0xD1: SET(CRegister, 2); op(2, 2); break;
	case 0xD9: SET(CRegister, 3); op(2, 2); break;
	case 0xE1: SET(CRegister, 4); op(2, 2); break;
	case 0xE9: SET(CRegister, 5); op(2, 2); break;
	case 0xF1: SET(CRegister, 6); op(2, 2); break;
	case 0xF9: SET(CRegister, 7); op(2, 2); break;

	case 0xC2: SET(DRegister, 0); op(2, 2); break;
	case 0xCA: SET(DRegister, 1); op(2, 2); break;
	case 0xD2: SET(DRegister, 2); op(2, 2); break;
	case 0xDA: SET(DRegister, 3); op(2, 2); break;
	case 0xE2: SET(DRegister, 4); op(2, 2); break;
	case 0xEA: SET(DRegister, 5); op(2, 2); break;
	case 0xF2: SET(DRegister, 6); op(2, 2); break;
	case 0xFA: SET(DRegister, 7); op(2, 2); break;

	case 0xC3: SET(ERegister, 0); op(2, 2); break;
	case 0xCB: SET(ERegister, 1); op(2, 2); break;
	case 0xD3: SET(ERegister, 2); op(2, 2); break;
	case 0xDB: SET(ERegister, 3); op(2, 2); break;
	case 0xE3: SET(ERegister, 4); op(2, 2); break;
	case 0xEB: SET(ERegister, 5); op(2, 2); break;
	case 0xF3: SET(ERegister, 6); op(2, 2); break;
	case 0xFB: SET(ERegister, 7); op(2, 2); break;

	case 0xC4: SET(HRegister, 0); op(2, 2); break;
	case 0xCC: SET(HRegister, 1); op(2, 2); break;
	case 0xD4: SET(HRegister, 2); op(2, 2); break;
	case 0xDC: SET(HRegister, 3); op(2, 2); break;
	case 0xE4: SET(HRegister, 4); op(2, 2); break;
	case 0xEC: SET(HRegister, 5); op(2, 2); break;
	case 0xF4: SET(HRegister, 6); op(2, 2); break;
	case 0xFC: SET(HRegister, 7); op(2, 2); break;

	case 0xC5: SET(LRegister, 0); op(2, 2); break;
	case 0xCD: SET(LRegister, 1); op(2, 2); break;
	case 0xD5: SET(LRegister, 2); op(2, 2); break;
	case 0xDD: SET(LRegister, 3); op(2, 2); break;
	case 0xE5: SET(LRegister, 4); op(2, 2); break;
	case 0xED: SET(LRegister, 5); op(2, 2); break;
	case 0xF5: SET(LRegister, 6); op(2, 2); break;
	case 0xFD: SET(LRegister, 7); op(2, 2); break;

	case 0xC6: SET(Pair(HRegister, LRegister).address(), 0); op(2, 4); break;
	case 0xCE: SET(Pair(HRegister, LRegister).address(), 1); op(2, 4); break;
	case 0xD6: SET(Pair(HRegister, LRegister).address(), 2); op(2, 4); break;
	case 0xDE: SET(Pair(HRegister, LRegister).address(), 3); op(2, 4); break;
	case 0xE6: SET(Pair(HRegister, LRegister).address(), 4); op(2, 4); break;
	case 0xEE: SET(Pair(HRegister, LRegister).address(), 5); op(2, 4); break;
	case 0xF6: SET(Pair(HRegister, LRegister).address(), 6); op(2, 4); break;
	case 0xFE: SET(Pair(HRegister, LRegister).address(), 7); op(2, 4); break;

	case 0x87: RES(ARegister, 0); op(2, 2); break;
	case 0x8F: RES(ARegister, 1); op(2, 2); break;
	case 0x97: RES(ARegister, 2); op(2, 2); break;
	case 0x9F: RES(ARegister, 3); op(2, 2); break;
	case 0xA7: RES(ARegister, 4); op(2, 2); break;
	case 0xAF: RES(ARegister, 5); op(2, 2); break;
	case 0xB7: RES(ARegister, 6); op(2, 2); break;
	case 0xBF: RES(ARegister, 7); op(2, 2); break;

	case 0x80: RES(BRegister, 0); op(2, 2); break;
	case 0x88: RES(BRegister, 1); op(2, 2); break;
	case 0x90: RES(BRegister, 2); op(2, 2); break;
	case 0x98: RES(BRegister, 3); op(2, 2); break;
	case 0xA0: RES(BRegister, 4); op(2, 2); break;
	case 0xA8: RES(BRegister, 5); op(2, 2); break;
	case 0xB0: RES(BRegister, 6); op(2, 2); break;
	case 0xB8: RES(BRegister, 7); op(2, 2); break;

	case 0x81: RES(CRegister, 0); op(2, 2); break;
	case 0x89: RES(CRegister, 1); op(2, 2); break;
	case 0x91: RES(CRegister, 2); op(2, 2); break;
	case 0x99: RES(CRegister, 3); op(2, 2); break;
	case 0xA1: RES(CRegister, 4); op(2, 2); break;
	case 0xA9: RES(CRegister, 5); op(2, 2); break;
	case 0xB1: RES(CRegister, 6); op(2, 2); break;
	case 0xB9: RES(CRegister, 7); op(2, 2); break;

	case 0x82: RES(DRegister, 0); op(2, 2); break;
	case 0x8A: RES(DRegister, 1); op(2, 2); break;
	case 0x92: RES(DRegister, 2); op(2, 2); break;
	case 0x9A: RES(DRegister, 3); op(2, 2); break;
	case 0xA2: RES(DRegister, 4); op(2, 2); break;
	case 0xAA: RES(DRegister, 5); op(2, 2); break;
	case 0xB2: RES(DRegister, 6); op(2, 2); break;
	case 0xBA: RES(DRegister, 7); op(2, 2); break;

	case 0x83: RES(ERegister, 0); op(2, 2); break;
	case 0x8B: RES(ERegister, 1); op(2, 2); break;
	case 0x93: RES(ERegister, 2); op(2, 2); break;
	case 0x9B: RES(ERegister, 3); op(2, 2); break;
	case 0xA3: RES(ERegister, 4); op(2, 2); break;
	case 0xAB: RES(ERegister, 5); op(2, 2); break;
	case 0xB3: RES(ERegister, 6); op(2, 2); break;
	case 0xBB: RES(ERegister, 7); op(2, 2); break;

	case 0x84: RES(HRegister, 0); op(2, 2); break;
	case 0x8C: RES(HRegister, 1); op(2, 2); break;
	case 0x94: RES(HRegister, 2); op(2, 2); break;
	case 0x9C: RES(HRegister, 3); op(2, 2); break;
	case 0xA4: RES(HRegister, 4); op(2, 2); break;
	case 0xAC: RES(HRegister, 5); op(2, 2); break;
	case 0xB4: RES(HRegister, 6); op(2, 2); break;
	case 0xBC: RES(HRegister, 7); op(2, 2); break;

	case 0x85: RES(LRegister, 0); op(2, 2); break;
	case 0x8D: RES(LRegister, 1); op(2, 2); break;
	case 0x95: RES(LRegister, 2); op(2, 2); break;
	case 0x9D: RES(LRegister, 3); op(2, 2); break;
	case 0xA5: RES(LRegister, 4); op(2, 2); break;
	case 0xAD: RES(LRegister, 5); op(2, 2); break;
	case 0xB5: RES(LRegister, 6); op(2, 2); break;
	case 0xBD: RES(LRegister, 7); op(2, 2); break;

	case 0x86: RES(Pair(HRegister, LRegister).address(), 0); op(2, 4); break;
	case 0x8E: RES(Pair(HRegister, LRegister).address(), 1); op(2, 4); break;
	case 0x96: RES(Pair(HRegister, LRegister).address(), 2); op(2, 4); break;
	case 0x9E: RES(Pair(HRegister, LRegister).address(), 3); op(2, 4); break;
	case 0xA6: RES(Pair(HRegister, LRegister).address(), 4); op(2, 4); break;
	case 0xAE: RES(Pair(HRegister, LRegister).address(), 5); op(2, 4); break;
	case 0xB6: RES(Pair(HRegister, LRegister).address(), 6); op(2, 4); break;
	case 0xBE: RES(Pair(HRegister, LRegister).address(), 7); op(2, 4); break;
	}
}

void CCpu::opcodeParse(Opcode code)
{
	Byte value = memory->readData(PCRegister + 1);
	Byte value2 = memory->readData(PCRegister + 2);

	switch (code)
	{

	case 0x7F: LD(ARegister, ARegister); op(1, 1); break;
	case 0x78: LD(ARegister, BRegister); op(1, 1); break;
	case 0x79: LD(ARegister, CRegister); op(1, 1); break;
	case 0x7A: LD(ARegister, DRegister); op(1, 1); break;
	case 0x7B: LD(ARegister, ERegister); op(1, 1); break;
	case 0x7C: LD(ARegister, HRegister); op(1, 1); break;
	case 0x7D: LD(ARegister, LRegister); op(1, 1); break;

	case 0x47: LD(BRegister, ARegister); op(1, 1); break;
	case 0x40: LD(BRegister, BRegister); op(1, 1); break;
	case 0x41: LD(BRegister, CRegister); op(1, 1); break;
	case 0x42: LD(BRegister, DRegister); op(1, 1); break;
	case 0x43: LD(BRegister, ERegister); op(1, 1); break;
	case 0x44: LD(BRegister, HRegister); op(1, 1); break;
	case 0x45: LD(BRegister, LRegister); op(1, 1); break;

	case 0x4F: LD(CRegister, ARegister); op(1, 1); break;
	case 0x48: LD(CRegister, BRegister); op(1, 1); break;
	case 0x49: LD(CRegister, CRegister); op(1, 1); break;
	case 0x4A: LD(CRegister, DRegister); op(1, 1); break;
	case 0x4B: LD(CRegister, ERegister); op(1, 1); break;
	case 0x4C: LD(CRegister, HRegister); op(1, 1); break;
	case 0x4D: LD(CRegister, LRegister); op(1, 1); break;

	case 0x57: LD(DRegister, ARegister); op(1, 1); break;
	case 0x50: LD(DRegister, BRegister); op(1, 1); break;
	case 0x51: LD(DRegister, CRegister); op(1, 1); break;
	case 0x52: LD(DRegister, DRegister); op(1, 1); break;
	case 0x53: LD(DRegister, ERegister); op(1, 1); break;
	case 0x54: LD(DRegister, HRegister); op(1, 1); break;
	case 0x55: LD(DRegister, LRegister); op(1, 1); break;

	case 0x5F: LD(ERegister, ARegister); op(1, 1); break;
	case 0x58: LD(ERegister, BRegister); op(1, 1); break;
	case 0x59: LD(ERegister, CRegister); op(1, 1); break;
	case 0x5A: LD(ERegister, DRegister); op(1, 1); break;
	case 0x5B: LD(ERegister, ERegister); op(1, 1); break;
	case 0x5C: LD(ERegister, HRegister); op(1, 1); break;
	case 0x5D: LD(ERegister, LRegister); op(1, 1); break;

	case 0x67: LD(HRegister, ARegister); op(1, 1); break;
	case 0x60: LD(HRegister, BRegister); op(1, 1); break;
	case 0x61: LD(HRegister, CRegister); op(1, 1); break;
	case 0x62: LD(HRegister, DRegister); op(1, 1); break;
	case 0x63: LD(HRegister, ERegister); op(1, 1); break;
	case 0x64: LD(HRegister, HRegister); op(1, 1); break;
	case 0x65: LD(HRegister, LRegister); op(1, 1); break;

	case 0x6F: LD(LRegister, ARegister); op(1, 1); break;
	case 0x68: LD(LRegister, BRegister); op(1, 1); break;
	case 0x69: LD(LRegister, CRegister); op(1, 1); break;
	case 0x6A: LD(LRegister, DRegister); op(1, 1); break;
	case 0x6B: LD(LRegister, ERegister); op(1, 1); break;
	case 0x6C: LD(LRegister, HRegister); op(1, 1); break;
	case 0x6D: LD(LRegister, LRegister); op(1, 1); break;

	case 0x3E: LD(ARegister, value); op(2, 2); break;
	case 0x06: LD(BRegister, value); op(2, 2); break;
	case 0x0E: LD(CRegister, value); op(2, 2); break;
	case 0x16: LD(DRegister, value); op(2, 2); break;
	case 0x1E: LD(ERegister, value); op(2, 2); break;
	case 0x26: LD(HRegister, value); op(2, 2); break;
	case 0x2E: LD(LRegister, value); op(2, 2); break;

	case 0x7E: LD(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;
	case 0x46: LD(BRegister, Pair(HRegister, LRegister).address()); op(1, 2); break;
	case 0x4E: LD(CRegister, Pair(HRegister, LRegister).address()); op(1, 2); break;
	case 0x56: LD(DRegister, Pair(HRegister, LRegister).address()); op(1, 2); break;
	case 0x5E: LD(ERegister, Pair(HRegister, LRegister).address()); op(1, 2); break;
	case 0x66: LD(HRegister, Pair(HRegister, LRegister).address()); op(1, 2); break;
	case 0x6E: LD(LRegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0x77: LD(Pair(HRegister, LRegister).address(), ARegister); op(1, 2); break;
	case 0x70: LD(Pair(HRegister, LRegister).address(), BRegister); op(1, 2); break;
	case 0x71: LD(Pair(HRegister, LRegister).address(), CRegister); op(1, 2); break;
	case 0x72: LD(Pair(HRegister, LRegister).address(), DRegister); op(1, 2); break;
	case 0x73: LD(Pair(HRegister, LRegister).address(), ERegister); op(1, 2); break;
	case 0x74: LD(Pair(HRegister, LRegister).address(), HRegister); op(1, 2); break;
	case 0x75: LD(Pair(HRegister, LRegister).address(), LRegister); op(1, 2); break;
	case 0x36: LD(Pair(HRegister, LRegister).address(), value); op(2, 3); break;

	case 0x0A: LD(ARegister, Pair(BRegister, CRegister).address()); op(1, 2); break;
	case 0x1A: LD(ARegister, Pair(DRegister, ERegister).address()); op(1, 2); break;
	case 0xF2: LD(ARegister, (Address)(0xFF00 + CRegister)); op(1, 2); break;

	case 0xE2: LD((Address)(0xFF00 + CRegister), ARegister); op(1, 2); break;
	case 0xF0: LD(ARegister, (Address)(0xFF00 + value)); op(2, 3); break;
	case 0xE0: LD((Address)(0xFF00 + value), ARegister); op(2, 3); break;
	case 0xFA: LD(ARegister, Pair(value2, value).address()); op(3, 4); break;

	case 0xEA: LD(Pair(value2, value).address(), ARegister); op(3, 4); break;
	case 0x2A: LD(ARegister, Pair(HRegister, LRegister).address()); Pair(HRegister, LRegister).increase(); op(1, 2); break;
	case 0x3A: LD(ARegister, Pair(HRegister, LRegister).address()); Pair(HRegister, LRegister).decrease(); op(1, 2); break;
	case 0x02: LD(Pair(BRegister, CRegister).address(), ARegister); op(1, 2); break;
	case 0x12: LD(Pair(DRegister, ERegister).address(), ARegister); op(1, 2); break;

	case 0x22: LD(Pair(HRegister, LRegister).address(), ARegister); Pair(HRegister, LRegister).increase(); op(1, 2); break;
	case 0x32: LD(Pair(HRegister, LRegister).address(), ARegister); Pair(HRegister, LRegister).decrease(); op(1, 2); break;

	case 0x01: LD(Pair(BRegister, CRegister), value2, value); op(3, 3); break;
	case 0x11: LD(Pair(DRegister, ERegister), value2, value); op(3, 3); break;
	case 0x21: LD(Pair(HRegister, LRegister), value2, value); op(3, 3); break;
	case 0x31: LD(SPRegister, value2, value); op(3, 3); break;
	case 0xF9: LD(SPRegister, HRegister, LRegister); op(1, 2); break;

	case 0xC5: PUSH(BRegister, CRegister); op(1, 4); break;
	case 0xD5: PUSH(DRegister, ERegister); op(1, 4); break;
	case 0xE5: PUSH(HRegister, LRegister); op(1, 4); break;
	case 0xF5: PUSH(ARegister, FRegister); op(1, 4); break;

	case 0xC1: POP(BRegister, CRegister); op(1, 3); break;
	case 0xD1: POP(DRegister, ERegister); op(1, 3); break;
	case 0xE1: POP(HRegister, LRegister); op(1, 3); break;
	case 0xF1: POP(ARegister, FRegister); FRegister &= 0xF0; op(1, 3); break;

	case 0xF8: LDHL(value); op(2, 3); break;
	case 0x08: LDNN(value, value2); op(3, 5); break;

	case 0x87: ADD(ARegister, ARegister); op(1, 1); break;
	case 0x80: ADD(ARegister, BRegister); op(1, 1); break;
	case 0x81: ADD(ARegister, CRegister); op(1, 1); break;
	case 0x82: ADD(ARegister, DRegister); op(1, 1); break;
	case 0x83: ADD(ARegister, ERegister); op(1, 1); break;
	case 0x84: ADD(ARegister, HRegister); op(1, 1); break;
	case 0x85: ADD(ARegister, LRegister); op(1, 1); break;
	case 0xC6: ADD(ARegister, value); op(2, 2); break;
	case 0x86: ADD(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0x8F: ADC(ARegister, ARegister); op(1, 1); break;
	case 0x88: ADC(ARegister, BRegister); op(1, 1); break;
	case 0x89: ADC(ARegister, CRegister); op(1, 1); break;
	case 0x8A: ADC(ARegister, DRegister); op(1, 1); break;
	case 0x8B: ADC(ARegister, ERegister); op(1, 1); break;
	case 0x8C: ADC(ARegister, HRegister); op(1, 1); break;
	case 0x8D: ADC(ARegister, LRegister); op(1, 1); break;
	case 0xCE: ADC(ARegister, value); op(2, 2); break;
	case 0x8E: ADC(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0x97: SUB(ARegister, ARegister); op(1, 1); break;
	case 0x90: SUB(ARegister, BRegister); op(1, 1); break;
	case 0x91: SUB(ARegister, CRegister); op(1, 1); break;
	case 0x92: SUB(ARegister, DRegister); op(1, 1); break;
	case 0x93: SUB(ARegister, ERegister); op(1, 1); break;
	case 0x94: SUB(ARegister, HRegister); op(1, 1); break;
	case 0x95: SUB(ARegister, LRegister); op(1, 1); break;
	case 0xD6: SUB(ARegister, value); op(2, 2); break;
	case 0x96: SUB(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0x9F: SBC(ARegister, ARegister); op(1, 1); break;
	case 0x98: SBC(ARegister, BRegister); op(1, 1); break;
	case 0x99: SBC(ARegister, CRegister); op(1, 1); break;
	case 0x9A: SBC(ARegister, DRegister); op(1, 1); break;
	case 0x9B: SBC(ARegister, ERegister); op(1, 1); break;
	case 0x9C: SBC(ARegister, HRegister); op(1, 1); break;
	case 0x9D: SBC(ARegister, LRegister); op(1, 1); break;
	case 0xDE: SBC(ARegister, value); op(2, 2); break;
	case 0x9E: SBC(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0xA7: AND(ARegister, ARegister); op(1, 1); break;
	case 0xA0: AND(ARegister, BRegister); op(1, 1); break;
	case 0xA1: AND(ARegister, CRegister); op(1, 1); break;
	case 0xA2: AND(ARegister, DRegister); op(1, 1); break;
	case 0xA3: AND(ARegister, ERegister); op(1, 1); break;
	case 0xA4: AND(ARegister, HRegister); op(1, 1); break;
	case 0xA5: AND(ARegister, LRegister); op(1, 1); break;
	case 0xE6: AND(ARegister, value); op(2, 2); break;
	case 0xA6: AND(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0xB7: OR(ARegister, ARegister); op(1, 1); break;
	case 0xB0: OR(ARegister, BRegister); op(1, 1); break;
	case 0xB1: OR(ARegister, CRegister); op(1, 1); break;
	case 0xB2: OR(ARegister, DRegister); op(1, 1); break;
	case 0xB3: OR(ARegister, ERegister); op(1, 1); break;
	case 0xB4: OR(ARegister, HRegister); op(1, 1); break;
	case 0xB5: OR(ARegister, LRegister); op(1, 1); break;
	case 0xF6: OR(ARegister, value); op(2, 2); break;
	case 0xB6: OR(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0xAF: XOR(ARegister, ARegister); op(1, 1); break;
	case 0xA8: XOR(ARegister, BRegister); op(1, 1); break;
	case 0xA9: XOR(ARegister, CRegister); op(1, 1); break;
	case 0xAA: XOR(ARegister, DRegister); op(1, 1); break;
	case 0xAB: XOR(ARegister, ERegister); op(1, 1); break;
	case 0xAC: XOR(ARegister, HRegister); op(1, 1); break;
	case 0xAD: XOR(ARegister, LRegister); op(1, 1); break;
	case 0xEE: XOR(ARegister, value); op(2, 2); break;
	case 0xAE: XOR(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0xBF: CP(ARegister, ARegister); op(1, 1); break;
	case 0xB8: CP(ARegister, BRegister); op(1, 1); break;
	case 0xB9: CP(ARegister, CRegister); op(1, 1); break;
	case 0xBA: CP(ARegister, DRegister); op(1, 1); break;
	case 0xBB: CP(ARegister, ERegister); op(1, 1); break;
	case 0xBC: CP(ARegister, HRegister); op(1, 1); break;
	case 0xBD: CP(ARegister, LRegister); op(1, 1); break;
	case 0xFE: CP(ARegister, value); op(2, 2); break;
	case 0xBE: CP(ARegister, Pair(HRegister, LRegister).address()); op(1, 2); break;

	case 0x3C: INC(ARegister); op(1, 1); break;
	case 0x04: INC(BRegister); op(1, 1); break;
	case 0x0C: INC(CRegister); op(1, 1); break;
	case 0x14: INC(DRegister); op(1, 1); break;
	case 0x1C: INC(ERegister); op(1, 1); break;
	case 0x24: INC(HRegister); op(1, 1); break;
	case 0x2C: INC(LRegister); op(1, 1); break;
	case 0x34: INC(Pair(HRegister, LRegister).address()); op(1, 3); break;

	case 0x3D: DEC(ARegister); op(1, 1); break;
	case 0x05: DEC(BRegister); op(1, 1); break;
	case 0x0D: DEC(CRegister); op(1, 1); break;
	case 0x15: DEC(DRegister); op(1, 1); break;
	case 0x1D: DEC(ERegister); op(1, 1); break;
	case 0x25: DEC(HRegister); op(1, 1); break;
	case 0x2D: DEC(LRegister); op(1, 1); break;
	case 0x35: DEC(Pair(HRegister, LRegister).address()); op(1, 3); break;

	case 0x09: ADDHL(Pair(BRegister, CRegister)); op(1, 2); break;
	case 0x19: ADDHL(Pair(DRegister, ERegister)); op(1, 2); break;
	case 0x29: ADDHL(Pair(HRegister, LRegister)); op(1, 2); break;
	case 0x39: ADDHLSP(); op(1, 2); break;
	case 0xE8: ADDSP(value); op(2, 4); break;

	case 0x03: INC(Pair(BRegister, CRegister)); op(1, 2); break;
	case 0x13: INC(Pair(DRegister, ERegister)); op(1, 2); break;
	case 0x23: INC(Pair(HRegister, LRegister)); op(1, 2); break;
	case 0x33: INCSP(); op(1, 2); break;

	case 0x0B: DEC(Pair(BRegister, CRegister)); op(1, 2); break;
	case 0x1B: DEC(Pair(DRegister, ERegister)); op(1, 2); break;
	case 0x2B: DEC(Pair(HRegister, LRegister)); op(1, 2); break;
	case 0x3B: DECSP(); op(1, 2); break;

	case 0x07: RL(ARegister, false);  op(1, 1); break;
	case 0x17: RL(ARegister, true);   op(1, 1); break;
	case 0x0F: RR(ARegister, false);  op(1, 1); break;
	case 0x1F: RR(ARegister, true);   op(1, 1); break;

	case 0xCB: bitOpParse(value); break;

	case 0xC3: op(3, 3); JP(Pair(value2, value));   break;
	case 0xC2: op(3, 3); JPNZ(Pair(value2, value)); break;
	case 0xCA: op(3, 3); JPZ(Pair(value2, value));  break;
	case 0xD2: op(3, 3); JPNC(Pair(value2, value)); break;
	case 0xDA: op(3, 3); JPC(Pair(value2, value));  break;

	case 0x18: op(2, 2); JR(value); break;
	case 0x20: op(2, 2); JRNZ(value); break;
	case 0x28: op(2, 2); JRZ(value); break;
	case 0x30: op(2, 2); JRNC(value); break;
	case 0x38: op(2, 2); JRC(value); break;
	case 0xE9: op(1, 1); JPHL(); break;

	case 0xCD: op(3, 3); CALL(value, value2); break;
	case 0xC4: op(3, 3); CALLNZ(value, value2); break;
	case 0xCC: op(3, 3); CALLZ(value, value2); break;
	case 0xD4: op(3, 3); CALLNC(value, value2); break;
	case 0xDC: op(3, 3); CALLC(value, value2); break;

	case 0xC9: op(1, 1); RET(); break;
	case 0xC0: op(1, 2); RETNZ(); break;
	case 0xC8: op(1, 2); RETZ(); break;
	case 0xD0: op(1, 2); RETNC(); break;
	case 0xD8: op(1, 2); RETC(); break;
	case 0xD9: op(1, 1); RETI(); break;

	case 0xC7: op(1, 4); RST(0x00); break;
	case 0xCF: op(1, 4); RST(0x08); break;
	case 0xD7: op(1, 4); RST(0x10); break;
	case 0xDF: op(1, 4); RST(0x18); break;
	case 0xE7: op(1, 4); RST(0x20); break;
	case 0xEF: op(1, 4); RST(0x28); break;
	case 0xF7: op(1, 4); RST(0x30); break;
	case 0xFF: op(1, 4); RST(0x38); break;

	case 0x27: DAA(); op(1, 1); break;
	case 0x2F: CPL(); op(1, 1); break;
	case 0x00: NOP(); op(1, 1); break;

	case 0xF3: DI(); op(1, 1); break;
	case 0xFB: EI(); op(1, 1); break;

	case 0x76: HALT(); op(1, 1); break;

	case 0x37: SCF(); op(1, 1); break;
	case 0x3F: CCF(); op(1, 1); break;

	default: op(1, 0); break;
	}
}