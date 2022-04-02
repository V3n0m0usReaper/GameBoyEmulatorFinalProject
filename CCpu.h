#pragma once

#include "CDataTypes.h"
#include "CMemory.h"


class CCpu
{
public:

	// these are the registers that will be used for the gameboy to allow it to store the relevant data
	Byte ARegister;
	Byte BRegister;
	Byte CRegister;
	Byte DRegister;
	Byte ERegister;
	Byte HRegister;
	Byte LRegister;

	//this is the flag register that is used when
	Byte FRegister;

	//these registers are used to stor four digit hexidecimal codes such as 0x0000
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
	

private:

	CMemory* memory;

	const int zeroFlag = 0b10000000;
	const int subtractionFlag = 0b01000000;
	const int flagHalfCarry = 0b00100000;
	const int flagCarry = 0b00010000;

	void op(int pc, int cycle);
	void bitOpParse(Opcode code);
	void flagSet(int flag, bool value);

	void LD(Byte& destination, Byte value);
	void LD(Byte& destination, Address address);
	void LD(Address address, Byte value);

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

	void ADD16(Byte16Bit target, Byte16Bit value);
	void ADDHL(Pair registerPair);
	void ADDHLSP();
	void ADDSP(Byte value);

	void INC(Pair registerPair);
	void INCSP();
	void DEC(Pair registerPair);
	void DECSP();

	void LeftShift(Byte& target);
	void LeftShift(Address address);

	void RightShift(Byte& target, bool includeTopBit);
	void RightShift(Address address, bool includeTopBit);

	void RL(Byte& target, bool carry, bool zeroFlag = false);
	void RL(Address address, bool carry);

	void RR(Byte& target, bool carry, bool zeroFlag = false);
	void RR(Address address, bool carry);

	void SRA(Byte& target);
	void SRA(Address address);

	void SLA(Byte& target);
	void SLA(Address address);

	void SWAP(Byte& target);
	void SWAP(Address address);

	void BIT(Byte target, int bit);
	void BIT(Address address, int bit);

	void SET(Byte& target, int bit);
	void SET(Address address, int bit); 

	void RES(Byte& target, int bit);
	void RES(Address address, int bit);

	void CCF();
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


