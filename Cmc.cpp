#include "Cmc.h"

void Cmc::initialise(vector<Byte> cartridge_buffer)
{
	CartridgeRom = cartridge_buffer;
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

void Cmc::save(ofstream& file) {
	cout << "did nothing" << endl;
}
void Cmc::load(ifstream& file) {
	cout << "did nothing" << endl;
}

Byte MemoryBankController0::readData(Address location)
{
	if (location >= 0x0000 && location <= 0x7FFF)
		return CartridgeRom[location];
	else if (location >= 0xA000 && location <= 0xBFFF)
		return ERAM[location & 0x1FFF];
	else
		return 0x00;
}

void MemoryBankController0::writeData(Address location, Byte data)
{
	if (location >= 0xA000 && location <= 0xBFFF)
		ERAM[location & 0x1FFF] = data;
}

Byte MemoryBankController1::readData(Address location)
{
	if (location >= 0x0000 && location <= 0x3FFF)
	{
		return CartridgeRom[location];
	}
	else if (location >= 0x4000 && location <= 0x7FFF)
	{
		Byte temp_id = RomBankID;

		int offset = location - 0x4000;
		int lookup = (temp_id * 0x4000) + offset;

		return CartridgeRom[lookup];
	}
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (EnableRamAccess == false)
			return 0xFF;

		Byte temp_id = (EnableRamBank) ? RamBankID : 0x00;

		int offset = location - 0xA000;
		int lookup = (temp_id * 0x2000) + offset;

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
		Byte bank_id = data & 0x1F;

		RomBankID = (RomBankID & 0xE0) | bank_id;

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
		Byte bank_id = data & 0x03;

		if (EnableRamBank)
		{
			RamBankID = bank_id;
		}
		else
		{
			RomBankID = RomBankID | (bank_id << 5);

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
		RamBankID = bitSet(data, BIT_0);
	}
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (EnableRamAccess)
		{
			int offset = location - 0xA000;
			int lookup = (RamBankID * 0x2000) + offset;

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

	cout << "wrote registers" << endl;
}

void MemoryBankController1::load(ifstream& file)
{
	file.read((char*)&RomBankID, sizeof(RomBankID));
	file.read((char*)&RamBankID, sizeof(RamBankID));
	file.read((char*)&EnableRamBank, sizeof(EnableRamBank));
	file.read((char*)&EnableRamAccess, sizeof(EnableRamAccess));
	file.read((char*)&mode, sizeof(mode));
	cout << "read registers" << endl;
}

Byte MemoryBankController2::readData(Address location) { return 0; }
void MemoryBankController2::writeData(Address location, Byte data) {}

Byte MemoryBankController3::readData(Address location)
{
	if (location >= 0x0000 && location <= 0x3FFF)
	{
		return CartridgeRom[location];
	}
	else if (location >= 0x4000 && location <= 0x7FFF)
	{
		int offset = location - 0x4000;
		int lookup = (RomBankID * 0x4000) + offset;

		return CartridgeRom[lookup];
	}
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (RTCEnabled)
			return 0x00;

		if (EnableRamAccess == false)
			return 0xFF;

		int offset = location - 0xA000;
		int lookup = (RamBankID * 0x2000) + offset;

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
			RomBankID++;
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
				return;

			int offset = location - 0xA000;
			int lookup = (RamBankID * 0x2000) + offset;

			ERAM[lookup] = data;
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
	cout << "wrote registers" << endl;
}

void MemoryBankController3::load(ifstream& file)
{
	file.read((char*)&RomBankID, sizeof(RomBankID));
	file.read((char*)&RamBankID, sizeof(RamBankID));
	file.read((char*)&EnableRamBank, sizeof(EnableRamBank));
	file.read((char*)&EnableRamAccess, sizeof(EnableRamAccess));
	file.read((char*)&mode, sizeof(mode));
	file.read((char*)&RTCEnabled, sizeof(RTCEnabled));
	cout << "read registers" << endl;
}