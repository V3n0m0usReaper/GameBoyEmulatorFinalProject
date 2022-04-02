#include "CCpu.h"

void CCpu::initialise(CMemory* _memory)
{
	memory = _memory;
	reset();
}


void CCpu::reset()
{
	ARegister = 0x01;
	BRegister = 0x00;
	CRegister = 0x13;
	DRegister = 0x00;
	ERegister = 0xD8;
	FRegister = 0xB0;
	HRegister = 0x01;
	LRegister = 0x4D;
	SPRegister = 0xFFFE;
	PCRegister = 0x100;
}

void CCpu::save(ofstream& file)
{
	file.write((char*)&ARegister, sizeof(ARegister));
	file.write((char*)&BRegister, sizeof(BRegister));
	file.write((char*)&CRegister, sizeof(CRegister));
	file.write((char*)&DRegister, sizeof(DRegister));
	file.write((char*)&ERegister, sizeof(ERegister));
	file.write((char*)&FRegister, sizeof(FRegister));
	file.write((char*)&HRegister, sizeof(HRegister));
	file.write((char*)&LRegister, sizeof(LRegister));
	file.write((char*)&SPRegister, sizeof(SPRegister));
	file.write((char*)&PCRegister, sizeof(PCRegister));
}

void CCpu::load(ifstream& file)
{
	file.read((char*)&ARegister, sizeof(ARegister));
	file.read((char*)&BRegister, sizeof(BRegister));
	file.read((char*)&CRegister, sizeof(CRegister));
	file.read((char*)&DRegister, sizeof(DRegister));
	file.read((char*)&ERegister, sizeof(ERegister));
	file.read((char*)&FRegister, sizeof(FRegister));
	file.read((char*)&HRegister, sizeof(HRegister));
	file.read((char*)&LRegister, sizeof(LRegister));
	file.read((char*)&SPRegister, sizeof(SPRegister));
	file.read((char*)&PCRegister, sizeof(PCRegister));
}

void CCpu::op(int pc, int cycle)
{
	PCRegister += pc;
	cycles += (cycle * 4);
}

void CCpu::flagSet(int flag, bool value)
{
	if (value == true)
	{
		FRegister |= flag;
	}
		
	else
	{
		FRegister &= ~(flag);
	}
		
}

void CCpu::LD(Byte& destination, Byte value)
{
	destination = value;
}

void CCpu::LD(Byte& destination, Address addr)
{
	destination = memory->readData(addr);
}

void CCpu::LD(Address addr, Byte value)
{
	memory->writeData(addr, value);
}

void CCpu::LD(Pair reg_pair, Byte upper, Byte lower)
{
	reg_pair.set(upper, lower);
}

void CCpu::LD(Byte16Bit& reg_pair, Byte upper, Byte lower)
{
	reg_pair = combineByte(upper, lower);
}

void CCpu::LDHL(Byte value)
{
	signedByte16Bit signed_val = (signedByte16Bit)(signedByte)value;
	Byte16Bit result = (Byte16Bit)((signedByte16Bit)SPRegister + signed_val);

	flagSet(flagCarry, (result & 0xFF) < (SPRegister & 0xFF)); 
	flagSet(flagHalfCarry, (result & 0xF) < (SPRegister & 0xF)); 
	flagSet(zeroFlag, false);
	flagSet(subtractionFlag, false);

	Pair(HRegister, LRegister).set(result);
}

void CCpu::LDNN(Byte low, Byte high)
{
	Byte lsb = lowByte(SPRegister);
	Byte msb = highByte(SPRegister);

	Address addr = Pair(high, low).address();

	LD(addr++, lsb);
	LD(addr, msb);
}

void CCpu::PUSH(Byte high, Byte low)
{
	memory->writeData(--SPRegister, high);
	memory->writeData(--SPRegister, low);
}

void CCpu::POP(Byte& high, Byte& low)
{
	low = memory->readData(SPRegister++);
	high = memory->readData(SPRegister++);
}

void CCpu::ADD(Byte& target, Byte value)
{
	Byte16Bit result = (Byte16Bit)target + (Byte16Bit)value;

	flagSet(flagHalfCarry, ((target & 0xF) + (value & 0xF)) > 0xF);
	flagSet(flagCarry, (result > 0xFF));
	flagSet(zeroFlag, (result & 0xFF) == 0);
	flagSet(subtractionFlag, false);

	target = (result & 0xFF);
}

void CCpu::ADD(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	ADD(target, val);
}

void CCpu::ADC(Byte& target, Byte value)
{
	Byte16Bit carry = (FRegister & flagCarry) ? 1 : 0;
	Byte16Bit result = (Byte16Bit)target + (Byte16Bit)value + carry;

	flagSet(flagHalfCarry, ((target & 0x0F) + (value & 0xF) + (Byte)carry) > 0x0F);
	flagSet(flagCarry, (result > 0xFF));
	flagSet(zeroFlag, (result & 0xFF) == 0);
	flagSet(subtractionFlag, false);

	target = (result & 0xFF);
}

void CCpu::ADC(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	ADC(target, val);
}

void CCpu::SUB(Byte& target, Byte value)
{
	signedByte16Bit result = (signedByte16Bit)target - (signedByte16Bit)value;

	signedByte16Bit s_target = (signedByte16Bit)target;
	signedByte16Bit s_value = (signedByte16Bit)value;

	flagSet(flagHalfCarry, (((s_target & 0xF) - (s_value & 0xF)) < 0));
	flagSet(flagCarry, result < 0);
	flagSet(subtractionFlag, true);
	flagSet(zeroFlag, (result & 0xFF) == 0);

	target = (Byte)(result & 0xFF);
}

void CCpu::SUB(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	SUB(target, val);
}

void CCpu::SBC(Byte& target, Byte value)
{
	Byte16Bit carry = (FRegister & flagCarry) ? 1 : 0;
	signedByte16Bit result = (signedByte16Bit)target - (signedByte16Bit)value - carry;

	signedByte16Bit s_target = (signedByte16Bit)target;
	signedByte16Bit s_value = (signedByte16Bit)value;

	flagSet(flagHalfCarry, (((s_target & 0xF) - (s_value & 0xF) - carry) < 0));
	flagSet(flagCarry, result < 0);
	flagSet(subtractionFlag, true);
	flagSet(zeroFlag, (result & 0xFF) == 0);

	target = (Byte)(result & 0xFF);
}

void CCpu::SBC(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	SBC(target, val);
}

void CCpu::AND(Byte& target, Byte value)
{
	target &= value;

	flagSet(zeroFlag, (target == 0));
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, true);
	flagSet(flagCarry, false);
}

void CCpu::AND(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	AND(target, val);
}

void CCpu::OR(Byte& target, Byte value)
{
	target |= value;

	flagSet(zeroFlag, (target == 0));
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, false);
	flagSet(flagCarry, false);
}

void CCpu::OR(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	OR(target, val);
}

void CCpu::XOR(Byte& target, Byte value)
{
	target ^= value;

	flagSet(zeroFlag, (target == 0));
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, false);
	flagSet(flagCarry, false);
}

void CCpu::XOR(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	XOR(target, val);
}


void CCpu::CP(Byte& target, Byte value)
{
	int result = target - value;

	flagSet(zeroFlag, (result == 0)); 
	flagSet(subtractionFlag, true); 
	flagSet(flagHalfCarry, (((target & 0xF) - (value & 0xF)) < 0));
	flagSet(flagCarry, (result < 0));
}

void CCpu::CP(Byte& target, Address addr)
{
	Byte val = memory->readData(addr);
	CP(target, val);
}

void CCpu::INC(Byte& target)
{
	Byte result = target + 1;
	flagSet(zeroFlag, (result == 0));
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, ((((target & 0xF) + 1) & 0x10) != 0));

	target = result;
}

void CCpu::INC(Address addr)
{
	Byte value = memory->readData(addr);
	INC(value);
	memory->writeData(addr, value);
}

void CCpu::DEC(Byte& target)
{
	Byte result = target - 1;
	flagSet(zeroFlag, (result == 0));
	flagSet(subtractionFlag, true);
	flagSet(flagHalfCarry, (((target & 0xF) - 1) < 0));

	target = result;
}

void CCpu::DEC(Address addr)
{
	Byte value = memory->readData(addr);
	DEC(value);
	memory->writeData(addr, value);
}

// 16-bit arithmetic

void CCpu::ADD16(Byte16Bit target, Byte16Bit value)
{
	int result = target + value;
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, ((((target & 0xFFF) + (value & 0xFFF)) & 0x1000) != 0));
	flagSet(flagCarry, (result > 0xFFFF));
}

void CCpu::ADDHL(Pair reg_pair)
{
	Byte16Bit target = Pair(HRegister, LRegister).get();
	Byte16Bit value = reg_pair.get();
	Byte16Bit result = target + value;

	ADD16(target, value);

	Pair(HRegister, LRegister).set(result);
}

void CCpu::ADDHLSP()
{
	Byte high = highByte(SPRegister);
	Byte low = lowByte(SPRegister);

	ADDHL(Pair(high, low));
}

void CCpu::ADDSP(Byte value)
{
	signedByte16Bit val_signed = (signedByte16Bit)(signedByte16Bit)value;
	Byte16Bit result = (Byte16Bit)((signedByte16Bit)SPRegister + val_signed);

	flagSet(flagCarry, (result & 0xFF) < (SPRegister & 0xFF));
	flagSet(flagHalfCarry, (result & 0xF) < (SPRegister & 0xF));
	flagSet(subtractionFlag, false);
	flagSet(zeroFlag, false);

	SPRegister = result;
}

void CCpu::INC(Pair reg_pair)
{
	reg_pair.set(reg_pair.get() + 1);
}

void CCpu::INCSP()
{
	SPRegister++;
}

void CCpu::DEC(Pair reg_pair)
{
	reg_pair.set(reg_pair.get() - 1); 
}

void CCpu::DECSP()
{
	SPRegister--;
}

void CCpu::RL(Byte& target, bool carry, bool zero_flag)
{
	int bit7 = ((target & 0x80) != 0);
	target = target << 1;

	target |= (carry) ? ((FRegister & flagCarry) != 0) : bit7;

	flagSet(zeroFlag, ((zero_flag) ? (target == 0) : false));
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, false);
	flagSet(flagCarry, (bit7 != 0));
}

void CCpu::RL(Address addr, bool carry)
{
	Byte value = memory->readData(addr);
	RL(value, carry, true);
	memory->writeData(addr, value);
}

void CCpu::RR(Byte& target, bool carry, bool zero_flag)
{
	int bit1 = ((target & 0x1) != 0);
	target = target >> 1;

	target |= (carry) ? (((FRegister& flagCarry) != 0) << 7) : (bit1 << 7);

	flagSet(zeroFlag, ((zero_flag) ? (target == 0) : false));
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, false);
	flagSet(flagCarry, (bit1 != 0));
}

void CCpu::RR(Address addr, bool carry)
{
	Byte value = memory->readData(addr);
	RR(value, carry, true);
	memory->writeData(addr, value);
}


void CCpu::LeftShift(Byte& target)
{
	Byte result = target << 1;

	flagSet(flagCarry, (target & 0x80) != 0);
	flagSet(flagHalfCarry, false);
	flagSet(subtractionFlag, false);
	flagSet(zeroFlag, (result == 0));

	target = result;
}

void CCpu::LeftShift(Address addr)
{
	Byte data = memory->readData(addr);
	LeftShift(data);
	memory->writeData(addr, data);
}


void CCpu::RightShift(Byte& target, bool include_top_bit)
{
	bool top_bit_set = bitSet(target, BIT_7);

	Byte result;

	if (include_top_bit)
		result = (top_bit_set) ? ((target >> 1) | 0x80) : (target >> 1);
	else
		result = target >> 1;

	flagSet(flagCarry, (target & 0x01) != 0);
	flagSet(flagHalfCarry, false);
	flagSet(subtractionFlag, false);
	flagSet(zeroFlag, (result == 0));

	target = result;
}

void CCpu::RightShift(Address addr, bool include_top_bit)
{
	Byte data = memory->readData(addr);
	RightShift(data, include_top_bit);
	memory->writeData(addr, data);
}

void CCpu::SRA(Byte& target)
{
	int bit7 = ((target & 0x80) != 0);
	RR(target, true);
	target |= (bit7 << 7);
	flagSet(zeroFlag, (target == 0));
}

void CCpu::SRA(Address addr)
{
	Byte value = memory->readData(addr);
	SRA(value);
	memory->writeData(addr, value);
}

void CCpu::SLA(Byte& target)
{
	RR(target, true, true);
}

void CCpu::SLA(Address addr)
{
	Byte value = memory->readData(addr);
	SLA(value);
	memory->writeData(addr, value);
}

void CCpu::SWAP(Byte& target)
{
	Byte first = target >> 4;
	Byte second = target << 4;

	Byte swapped = first | second;

	target = swapped;

	flagSet(flagCarry, false);
	flagSet(flagHalfCarry, false);
	flagSet(subtractionFlag, false);
	flagSet(zeroFlag, (target == 0));
}

void CCpu::SWAP(Address addr)
{
	Byte value = memory->readData(addr);
	SWAP(value);
	memory->writeData(addr, value);
}

void CCpu::BIT(Byte target, int bit)
{
	flagSet(zeroFlag, (((1 << bit) & ~target) != 0));
	flagSet(flagHalfCarry, true);
	flagSet(subtractionFlag, false);
}

void CCpu::BIT(Address addr, int bit)
{
	Byte value = memory->readData(addr);
	BIT(value, bit);
}

void CCpu::SET(Byte& target, int bit)
{
	target = (target | (1 << bit));
}

void CCpu::SET(Address addr, int bit)
{
	Byte value = memory->readData(addr);
	SET(value, bit);
	memory->writeData(addr, value);
}

void CCpu::RES(Byte& target, int bit)
{
	target = (~(1 << bit) & target);
}

void CCpu::RES(Address addr, int bit)
{
	Byte value = memory->readData(addr);
	RES(value, bit);
	memory->writeData(addr, value);
}

void CCpu::JP(Pair target)
{
	PCRegister = target.address();
	op(0, 1);
}

void CCpu::JPNZ(Pair target)
{
	if ((FRegister & zeroFlag) == 0)
		JP(target);
}

void CCpu::JPZ(Pair target)
{
	if ((FRegister & zeroFlag) != 0)
		JP(target);
}

void CCpu::JPNC(Pair target)
{
	if ((FRegister & flagCarry) == 0)
		JP(target);
}

void CCpu::JPC(Pair target)
{
	if ((FRegister & flagCarry) != 0)
		JP(target);
}


void CCpu::JR(Byte value)
{
	signedByte signed_val = ((signedByte)(value));
	PCRegister += signed_val;
	op(0, 1);
}

void CCpu::JRNZ(Byte value)
{
	if ((FRegister & zeroFlag) == 0)
		JR(value);
}

void CCpu::JRZ(Byte value)
{
	if ((FRegister & zeroFlag) != 0)
		JR(value);
}

void CCpu::JRNC(Byte value)
{
	if ((FRegister & flagCarry) == 0)
		JR(value);
}

void CCpu::JRC(Byte value)
{
	if ((FRegister & flagCarry) != 0)
		JR(value);
}

void CCpu::JPHL()
{
	PCRegister = Pair(HRegister, LRegister).address();
}

void CCpu::CALL(Byte low, Byte high)
{
	memory->writeData(--SPRegister, highByte(PCRegister));
	memory->writeData(--SPRegister, lowByte(PCRegister));

	JP(Pair(high, low));
	op(0, 3);
}

void CCpu::CALLNZ(Byte low, Byte high)
{
	if ((FRegister & zeroFlag) == 0)
		CALL(low, high);
}

void CCpu::CALLZ(Byte low, Byte high)
{
	if ((FRegister & zeroFlag) != 0)
		CALL(low, high);
}

void CCpu::CALLNC(Byte low, Byte high)
{
	if ((FRegister & flagCarry) == 0)
		CALL(low, high);
}

void CCpu::CALLC(Byte low, Byte high)
{
	if ((FRegister & flagCarry) != 0)
		CALL(low, high);
}

void CCpu::RET()
{
	Byte low = memory->readData(SPRegister++);
	Byte high = memory->readData(SPRegister++);

	PCRegister = Pair(high, low).get();
	op(0, 3);
}

void CCpu::RETI()
{
	masterInterupt = true;
	RET();
}

void CCpu::RETNZ()
{
	if ((FRegister & zeroFlag) == 0)
	{
		RET();
		op(0, 2);
	}
}

void CCpu::RETZ()
{
	if ((FRegister & zeroFlag) != 0)
	{
		RET();
		op(0, 2);
	}
}

void CCpu::RETNC()
{
	if ((FRegister & flagCarry) == 0)
	{
		RET();
		op(0, 2);
	}
}

void CCpu::RETC()
{
	if ((FRegister & flagCarry) != 0)
	{
		RET();
		op(0, 2);
	}
}

void CCpu::RST(Address addr)
{
	memory->writeData(--SPRegister, highByte(PCRegister));
	memory->writeData(--SPRegister, lowByte(PCRegister));

	PCRegister = addr;
}

void CCpu::DAA()
{
	Byte high = highNibble(ARegister);
	Byte low = lowNibble(ARegister);

	bool add = ((FRegister & subtractionFlag) == 0);
	bool carry = ((FRegister & flagCarry) != 0);
	bool half_carry = ((FRegister & flagHalfCarry) != 0);

	Byte16Bit result = (Byte16Bit)ARegister;
	Byte16Bit correction = (carry) ? 0x60 : 0x00;

	if (half_carry || (add) && ((result & 0x0F) > 9))
		correction |= 0x06;

	if (carry || (add) && (result > 0x99))
		correction |= 0x60;

	if (add)
		result += correction;
	else
		result -= correction;

	if (((correction << 2) & 0x100) != 0)
		flagSet(flagCarry, true);

	flagSet(flagHalfCarry, false);
	ARegister = (Byte)(result & 0xFF);
	flagSet(zeroFlag, ARegister == 0);
}

void CCpu::CPL()
{
	ARegister = ~ARegister;
	flagSet(flagHalfCarry, true);
	flagSet(subtractionFlag, true);
}

void CCpu::SCF()
{
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, false);
	flagSet(flagCarry, true);
}

void CCpu::CCF()
{
	flagSet(subtractionFlag, false);
	flagSet(flagHalfCarry, false);
	flagSet(flagCarry, ((FRegister & flagCarry) ? 1 : 0) ^ 1);
}

void CCpu::NOP()
{
}

void CCpu::HALT()
{
	cpuHalted = true;
	op(-1, 0); 
}

void CCpu::STOP()
{
}

void CCpu::DI()
{
	masterInterupt = false;
}

void CCpu::EI()
{
	masterInterupt = true;
}

void CCpu::debug()
{
	opcodeParse(0xE8);
}