#pragma once
#include "CDataTypes.h"

class Cmc
{
protected:
	vector<Byte> CartridgeRom;
	vector<Byte> ERAM;

	Byte RomBankID = 1;
	Byte RamBankID = 0;

	bool EnableRamBank = false;
	bool EnableRamAccess = false;

	Byte mode = 0;
	const Byte RomMode = 0;
	const Byte RamMode = 1;

public: 
	void initialise(vector<Byte> cartridgeBuffer);
	virtual Byte readData(Address addressLocation) = 0;
	virtual void writeData(Address addressLocation, Byte data) = 0;

	vector<Byte> getRam();
	void setRam(vector<Byte> data);
	virtual void save(ofstream& file);
	virtual void load(ifstream& file);
};

class MemoryBankController0 : public Cmc {
	Byte readData(Address addressLocation);
	void writeData(Address addressLocation, Byte data);
};

class MemoryBankController1 : public Cmc {
	Byte readData(Address addressLocation);
	void writeData(Address addressLocation, Byte data);
	void save(ofstream& file);
	void load(ifstream& file);
};

class MemoryBankController2 : public Cmc {
	Byte readData(Address addressLocation);
	void writeData(Address addressLocation, Byte data);
};

class MemoryBankController3 : public Cmc {
	bool RTCEnabled = false;

	Byte readData(Address addressLocation);
	void writeData(Address addressLocation, Byte data);
	void save(ofstream& file);
	void load(ifstream& file);
};

