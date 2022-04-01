#include "Cmc.h"

void Cmc::initialise(vector<Byte> cartridgeBuffer)
{
	CartridgeRom = cartridgeBuffer;
	ERAM = vector<Byte>(0x8000);
}

vector<Byte> Cmc::getRam()
{
	return ERAM;
}

void Cmc::setRam(vector<Byte> data)
{
	ERAM = data;
}

void Cmc::save(ofstream& file)
{
	cout << "save failed" << endl;
}

void Cmc::load(ifstream& file)
{
	cout << "load failed" << endl;
}


//this is the memory controller for any roms that use 32kb of space and dont need a memory controller
Byte MemoryBankController0::readData(Address location)
{
	if (location >= 0x0000 && location <= 0x7FFF)
	{
		return CartridgeRom[location];
	}
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		return ERAM[location & 0x1FFF];
	}
	else
	{
		return 0x00;
	}
}

void MemoryBankController0::writeData(Address location, Byte data)
{
	if (location >= 0xA000 && location <= 0xBFFF)
	{
		ERAM[location & 0x1FFF] = data;
	}
}

// this is the code for the memory controller 1 game roms. 

Byte MemoryBankController1::readData(Address location)
{
	if (location >= 0x0000 && location <= 0x3FFF)
	{
		return CartridgeRom[location];
	}
	else if (location >= 0x4000 && location <= 0x7FFF)
	{
		Byte tempID = RomBankID;

		int offsetLocation = location - 0x4000;
		int lookup = (tempID * 0x4000) + offsetLocation;

		return CartridgeRom[lookup];
	}

	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (EnableRamAccess == false)
		{
			return 0xFF;
		}
		Byte tempID = (EnableRamBank) ? RamBankID : 0x00;

		int offsetLocation = location - 0xA000;
		int lookup = (tempID * 0x2000) + offsetLocation;

		return ERAM[lookup];
	}
}

void MemoryBankController1::writeData(Address location, Byte data)
{
	if (location >= 0x0000 && location <= 0x1FFF)
	{
		EnableRamAccess = ((data & 0x0A) > 0) ? true : false;
	}

	else if (location >= 0x2000 && location <= 0x3FFF)
	{
		Byte bankID = data & 0x1F;

		RomBankID = (RomBankID & 0xE0) | bankID;

		switch (RomBankID)
		{
		case 0x00:
		case 0x20:
		case 0x40:
		case 0x60:
			RomBankID++;
			break;
		}

	}
	else if (location >= 0x4000 && location <= 0x5FFF)
	{
		Byte bankID = data & 0x03;

		if (EnableRamBank)
		{
			RamBankID = bankID;
		}
		else
		{
			RomBankID = RomBankID | (bankID << 5);

			switch (RomBankID)
			{
			case 0x00:
			case 0x20:
			case 0x40:
			case 0x60:
				RomBankID++;
				break;
			}
		}
	}

	else if (location >= 0x6000 && location <= 0x7FFF)
	{
		EnableRamBank = bitSet(data, BIT_0);
	}

	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (EnableRamAccess)
		{
			int offsetLocation = location - 0xA000;
			int lookup = (RamBankID * 0x2000) + offsetLocation;


			ERAM[lookup] = data;
		}
	}
}

void MemoryBankController1::save(ofstream& file)
{
	file.write((char*)&RomBankID, sizeof(RomBankID));
	file.write((char*)&RamBankID, sizeof(RamBankID));
	file.write((char*)&EnableRamBank, sizeof(EnableRamBank));
	file.write((char*)&EnableRamAccess, sizeof(EnableRamAccess));
	file.write((char*)&mode, sizeof(mode));

	cout << "Saved register data" << endl;
}

void MemoryBankController1::load(ifstream& file)
{
	file.read((char*)&RomBankID, sizeof(RomBankID));
	file.read((char*)&RamBankID, sizeof(RamBankID));
	file.read((char*)&EnableRamBank, sizeof(EnableRamBank));
	file.read((char*)&EnableRamAccess, sizeof(EnableRamAccess));
	file.read((char*)&mode, sizeof(mode));

	cout << "Loaded register data" << endl;
}

Byte MemoryBankController3::readData(Address location)
{
	if (location >= 0x0000 && location <= 0x3FFF)
	{
		return CartridgeRom[location];
	}
	else if (location >= 0x4000 && location <= 0x7FFF)
	{
		int offsetLocation = location - 0x4000;
		int lookup = (RomBankID * 0x4000) + offsetLocation;

		return CartridgeRom[lookup];
	}
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (RTCEnabled)
		{
			return 0x00;
		}
		if (EnableRamAccess == false)
		{
			return 0xFF;
		}
		int offsetLocation = location - 0xA000;
		int lookup = (RamBankID * 0x2000) + offsetLocation;

		return ERAM[lookup];
	}
}

void MemoryBankController3::writeData(Address location, Byte data)
{
	if (location >= 0x0000 && location <= 0x1FFF)
	{
		if ((data & 0x0A) > 0)
		{
			EnableRamAccess = true;
			RTCEnabled = true;
		}
		else
		{
			EnableRamAccess = false;
			RTCEnabled = false;
		}
	}
	else if (location >= 0x2000 && location <= 0x3FFF)
	{
		RomBankID = data & 0x7F;
		if (RomBankID == 0)
		{
			RomBankID++;
		}
	}
	else if (location >= 0x4000 && location <= 0x5FFF)
	{
		if (data <= 0x3)
		{
			RTCEnabled = false;
			RamBankID = data;
		}
		else if (data >= 0x08 && data <= 0x0C)
		{
			RTCEnabled = true;
		}

	}
	else if (location >= 0x6000 && location <= 0x7FFF)
	{

	}
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (!RTCEnabled)
		{
			if (!EnableRamAccess)
			{
				return;
			}
			int offsetLocation = location - 0xA000;
			int lookup = (RamBankID * 0x2000) + offsetLocation;

			ERAM[lookup];
		}
		else
		{

		}
	}
}

void MemoryBankController3::save(ofstream& file)
{
	file.write((char*)&RomBankID, sizeof(RomBankID));
	file.write((char*)&RamBankID, sizeof(RamBankID));
	file.write((char*)&EnableRamBank, sizeof(EnableRamBank));
	file.write((char*)&EnableRamAccess, sizeof(EnableRamAccess));
	file.write((char*)&mode, sizeof(mode));
	file.write((char*)&RTCEnabled, sizeof(RTCEnabled));

	cout << "Save MBC3 registers"<<endl;
}

void MemoryBankController3::load(ifstream& file)
{
	file.read((char*)&RomBankID, sizeof(RomBankID));
	file.read((char*)&RamBankID, sizeof(RamBankID));
	file.read((char*)&EnableRamBank, sizeof(EnableRamBank));
	file.read((char*)&EnableRamAccess, sizeof(EnableRamAccess));
	file.read((char*)&mode, sizeof(mode));
	file.read((char*)&RTCEnabled, sizeof(RTCEnabled));

	cout << "Load MBC3 registers" << endl;
}
Byte MemoryBankController2::readData(Address location)
{
	return 0;
}

void MemoryBankController2::writeData(Address location, Byte data)
{}