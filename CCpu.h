#pragma once

#include "CDataTypes.h"
#include "CMemory.h"


class CCpu
{
public:
	Byte ARegister;
	Byte BRegister;
	Byte CRegister;
	Byte DRegister;
	Byte ERegister;
	Byte HRegister;
	Byte LRegister;
	Byte FRegister;
	Byte16Bit SPRegister;
	Byte16Bit PCRegister;

	int ClockSpeed = 4194304; // this is because the cpu runs at 4194304 Hz
	int cycles = 0;
	bool masterInterupt = true;
	bool cpuHalted = false;

	void save(ofstream& file);
	void load(ifstream& file);


	void initialise(CMemory* _memory);
	void reset();
	void opcodeParse(Opcode code);
	void debug();
	CMemory* memory;

private:

	

	const int zeroFlag = 0b10000000;
	const int subtractionFlag = 0b01000000;
	const int flagHalfCarry = 0b00100000;
	const int flagCarry = 0b00010000;

	void op(int pc, int cycle);
	void bitOpParse(Opcode code);
	void flagSet(int flag, bool value);


	// these are the 8 bit loaders
	void LD(Byte& destination, Byte value);
	void LD(Byte& destination, Address address);
	void LD(Address address, Byte value);

	// these are the 16 bit loaders
	void LD(Pair registerPair, Byte upper, Byte lower);
	void LD(Byte16Bit& registerPair, Byte upper, Byte lower);
	void LDHL(Byte value);
	void LDNN(Byte low, Byte high);

	void PUSH(Byte low, Byte high);
	void POP(Byte& low, Byte& high);
	
	void ADD(Byte& target, Byte value);
	void ADD(Byte& target, Address address);
	
	void ADC(Byte& target, Byte value);
	void ADC(Byte& target, Address address);

	void SUB(Byte& target, Byte value);
	void SUB(Byte& target, Address address);

	void SBC(Byte& target, Byte value);
	void SBC(Byte& target, Address address);

	void AND(Byte& target, Byte value);
	void AND(Byte& target, Address address);

	void OR(Byte& target, Byte value);
	void OR(Byte& target, Address address);

	void XOR(Byte& target, Byte value);
	void XOR(Byte& target, Address address);

	void CP(Byte& target, Byte value);
	void CP(Byte& target, Address address);

	void INC(Byte& target);
	void INC(Address address);

	void DEC(Byte& target);
	void DEC(Address address);

	//these are the arithmatic funtions
	void ADD16(Byte16Bit target, Byte16Bit value);
	void ADDHL(Pair registerPair);
	void ADDHLSP();
	void ADDSP(Byte value);

	void INC(Pair registerPair);
	void INCSP();
	void DEC(Pair registerPair);
	void DECSP();

	//these are the shift function that will be used to display the screen
	void LeftShift(Byte& target);
	void LeftShift(Address address);

	void RightShift(Byte& target, bool includeTopBit);
	void RightShift(Address address, bool includeTopBit);

	//rotates left through the carry flag
	void RL(Byte& target, bool carry, bool zeroFlag = false);
	void RL(Address address, bool carry);

	//rotates right though carry flag
	void RR(Byte& target, bool carry, bool zeroFlag = false);
	void RR(Address address, bool carry);

	//shifts right into carry
	void SRA(Byte& target);
	void SRA(Address address);

	//shifts left into carry
	void SLA(Byte& target);
	void SLA(Address address);

	//swaps upper and lower nibbles
	void SWAP(Byte& target);
	void SWAP(Address address);

	//test bit in register
	void BIT(Byte target, int bit);
	void BIT(Address address, int bit);

	//set bit in register
	void SET(Byte& target, int bit);
	void SET(Address address, int bit); 

	//reset bit register
	void RES(Byte& target, int bit);
	void RES(Address address, int bit);

	//compliment carry flag
	void CCF();

	//set carry flag
	void SCF();

	void JP(Pair target);
	void JPNZ(Pair target);
	void JPZ(Pair target);
	void JPNC(Pair target);
	void JPC(Pair target);
	void JR(Byte value);

	void JRNZ(Byte value);
	void JRZ(Byte value);
	void JRNC(Byte value);
	void JRC(Byte value);
	void JPHL();

	void CALL(Byte low, Byte high);
	void CALLNZ(Byte low, Byte high);
	void CALLZ(Byte low, Byte high);
	void CALLNC(Byte low, Byte high);
	void CALLC(Byte low, Byte high);

	void RET();
	void RETI();
	void RETNZ();
	void RETZ();
	void RETNC();
	void RETC();

	void RST(Address address);
	void DAA();
	void CPL();
	void NOP();

	void HALT();
	void STOP();

	void DI();
	void EI();

	    
};


