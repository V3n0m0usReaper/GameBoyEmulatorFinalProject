#include "CMemory.h"
CMemory::CMemory()
{
	WRAM = vector<Byte>(0x2000);
	ZRAM = vector<Byte>(0x0100);
	VRAM = vector<Byte>(0x2000);
	OAM = vector<Byte>(0x0100);

	P1 = MemoryRegister(&ZRAM[0x00]);
	DIV = MemoryRegister(&ZRAM[0x04]);
	TIMA = MemoryRegister(&ZRAM[0x05]);
	TMA = MemoryRegister(&ZRAM[0x06]);
	TAC = MemoryRegister(&ZRAM[0x07]);
	LCDC = MemoryRegister(&ZRAM[0x40]);
	STAT = MemoryRegister(&ZRAM[0x41]);
	SCY = MemoryRegister(&ZRAM[0x42]);
	SCX = MemoryRegister(&ZRAM[0x43]);
	LY = MemoryRegister(&ZRAM[0x44]);
	LYC = MemoryRegister(&ZRAM[0x45]);
	DMA = MemoryRegister(&ZRAM[0x46]);
	BGP = MemoryRegister(&ZRAM[0x47]);
	OBP0 = MemoryRegister(&ZRAM[0x48]);
	OBP1 = MemoryRegister(&ZRAM[0x49]);
	WY = MemoryRegister(&ZRAM[0x4A]);
	WX = MemoryRegister(&ZRAM[0x4B]);
	IF = MemoryRegister(&ZRAM[0x0F]);
	IE = MemoryRegister(&ZRAM[0xFF]);

	reset();
}
void CMemory::reset()
{
	fill(WRAM.begin(), WRAM.end(), 0);
	fill(ZRAM.begin(), ZRAM.end(), 0);
	fill(VRAM.begin(), VRAM.end(), 0);
	fill(OAM.begin(), OAM.end(), 0);

	P1.set(0x00);
	DIV.set(0x00);
	TIMA.set(0x00);
	TMA.set(0x00);
	TAC.set(0x00);
	LCDC.set(0x91);
	SCY.set(0x00);
	SCX.set(0x00);
	LYC.set(0x00);
	BGP.set(0xFC);
	OBP0.set(0xFF);
	OBP1.set(0xFF);
	WY.set(0x00);
	WX.set(0x00);
	IF.set(0x00);
	IE.set(0x00);

	jpButtons = 0xF;
	jpArrows = 0xF;
}

void CMemory::romLoad(std::string location)
{
	ifstream input(location, ios::binary);
	vector<Byte> buffer((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));

	string title = "";

	for (int i = 0x0134; i <= 0x142; i++)
	{
		Byte character = buffer[i];
		if (character == 0)
		{
			break;
		}
		else
		{
			title.push_back(tolower(character));
		}
	}

	rom = title;

	cout << "Game name: " << title << endl;
	Byte gbType = buffer[0x0143];
	cout << "Gameboy Type: " << ((gbType == 0x80) ? "Gameboy Color" : "Gameboy") << endl;
	Byte functions = buffer[0x0146];
	cout << "Use " << ((functions == 0x3) ? "Super" : "") << "Gameboy functions" << endl;

	string cartridgeType[0x100];
	cartridgeType[0x0] = "ROM ONLY";
	cartridgeType[0x1] = "ROM+MBC1";
	cartridgeType[0x2] = "ROM+MBC1+RAM";
	cartridgeType[0x3] = "ROM+MBC1+RAM+BATT";
	cartridgeType[0x5] = "ROM+MBC2";
	cartridgeType[0x6] = "ROM+MBC2+BATTERY";
	cartridgeType[0x8] = "ROM+RAM";
	cartridgeType[0x9] = "ROM+RAM+BATTERY";
	cartridgeType[0xB] = "ROM+MMM01";
	cartridgeType[0xC] = "ROM+MMM01+SRAM";
	cartridgeType[0xD] = "ROM+MMM01+SRAM+BATT";
	cartridgeType[0xF] = "ROM+MBC3+TIMER+BATT";
	cartridgeType[0x10] = "ROM+MBC3+TIMER+RAM+BATT";
	cartridgeType[0x11] = "ROM+MBC3";
	cartridgeType[0x12] = "ROM+MBC3+RAM";
	cartridgeType[0x13] = "ROM+MBC3+RAM+BATT";
	cartridgeType[0x19] = "ROM+MBC5";
	cartridgeType[0x1A] = "ROM+MBC5+RAM";
	cartridgeType[0x1B] = "ROM+MBC5+RAM+BATT";
	cartridgeType[0x1C] = "ROM+MBC5+RUMBLE";
	cartridgeType[0x1D] = "ROM+MBC5+RUMBLE+SRAM";
	cartridgeType[0x1E] = "ROM+MBC5+RUMBLE+SRAM+BATT";
	cartridgeType[0x1F] = "Pocket Camera";
	cartridgeType[0xFD] = "Bandai TAMA5";
	cartridgeType[0xFE] = "Hudson HuC-3";
	cartridgeType[0xFF] = "Hudson HuC-1";


	Byte cartridge = buffer[0x0147];
	cout << "Cartridge Type: " << cartridgeType[cartridge] << endl;

	delete controller;

	switch (cartridge)
	{
	case 0x01:
	case 0x02:
	case 0x03:
		controller = new MemoryBankController1();
		break;
	case 0x05:
	case 0x06:
		cout << "MBC2 not implemented" << endl;
		controller = new MemoryBankController2();
		break;
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		controller = new MemoryBankController3();
		break;
	default:
		controller = new MemoryBankController0();
		break;
	}

	controller->initialise(buffer);

	Byte rsize = buffer[0x0148];
	cout << "ROM size: " << (32 << rsize) << "kB " << pow(2, rsize + 1) << " banks" << endl;
	int size;
	int banks;
	switch (buffer[0x149])
	{
	case 1:
		size = 2;
		banks = 1;
	case 2:
		size = 8;
		banks = 1;
	case 3:
		size = 32;
		banks = 4;
	case 4:
		size = 128;
		banks = 16;
	default:
		size = 0;
		banks = 0;
	}
	cout << "RAM size: " << size << "kB " << banks << " banks" << endl;
	cout << "Destination Code: " << (buffer[0x014A] == 1 ? "Non-" : "") << "Japanese" << endl;
}

void CMemory::save(ofstream& file)
{
	vWrite(file, VRAM);
	vWrite(file, OAM);
	vWrite(file, WRAM);
	vWrite(file, ZRAM);

	vector<Byte> eram = controller->getRam();
	vWrite(file, eram);
	controller->save(file);
}

void CMemory::load(ifstream& file)
{
	vLoad(file, VRAM);
	vLoad(file, OAM);
	vLoad(file, WRAM);
	vLoad(file, ZRAM);

	vector<Byte> eram(0x8000);

	vLoad(file, eram);
	controller->setRam(eram);
	controller->load(file);
}

void CMemory::vWrite(ofstream& file, vector<Byte>& vec)
{
	file.write((char*)&vec[0], vec.size());
}

void CMemory::vLoad(ifstream& file, vector<Byte>& vec)
{
	file.read((char*)&vec[0], vec.size());
}

void CMemory::dmaTransfer()
{
	Byte16Bit address = DMA.get() << 8;

	for (int i = 0; i < 0xA0; i++)
	{
		writeData((0xFE00 + i), readData(address + i));
	}

}

Byte CMemory::joypadState()
{
	Byte request = P1.get();

	switch (request)
	{
	case 0x10:
		return jpButtons;
	case 0x20:
		return jpArrows;
	default:
		return 0xFF;
	}
}

Byte CMemory::readData(Address location)
{
	switch (location & 0xF000)
	{
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return controller->readData(location);

	case 0x8000:
	case 0x9000:
		return VRAM[location & 0x1FFF];
	case 0xA000:
	case 0xB000:
		return controller->readData(location);

	case 0xC000:
	case 0xD000:
	case 0xE000:
		return WRAM[location & 0x1FFF];
	case 0xF000:
		switch (location & 0x0F00)
		{
		case 0x000:
		case 0x100:
		case 0x200:
		case 0x300:
		case 0x400:
		case 0x500:
		case 0x600:
		case 0x700:
		case 0x800:
		case 0x900:
		case 0xA00:
		case 0xB00:
		case 0xC00:
		case 0xD00:
			return WRAM[location & 0x1FFF];

		case 0xE00:
			return OAM[location & 0xFF];

		case 0xF00:
			if (location == 0xFF00)
			{
				return joypadState();
			}
			else
			{
				return ZRAM[location & 0xFF];
			}
		}
	default:
		return 0xFF;
	}
}

void CMemory::writeData(Address location, Byte data)
{
	switch (location & 0xF000)
	{
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		 controller->writeData(location, data);
		 break;

	case 0x8000:
	case 0x9000:
		 VRAM[location & 0x1FFF] = data;
		 break;
	case 0xA000:
	case 0xB000:
		 controller->writeData(location, data);
		 break;

	case 0xC000:
	case 0xD000:
	case 0xE000:
		 WRAM[location & 0x1FFF] = data;
	case 0xF000:
		switch (location & 0x0F00)
		{
		case 0x000:
		case 0x100:
		case 0x200:
		case 0x300:
		case 0x400:
		case 0x500:
		case 0x600:
		case 0x700:
		case 0x800:
		case 0x900:
		case 0xA00:
		case 0xB00:
		case 0xC00:
		case 0xD00:
			 WRAM[location & 0x1FFF] = data;
			 break;

		case 0xE00:
			 OAM[location & 0xFF] = data;

		case 0xF00:
			zeroPageWrite(location, data);
			break;
		}
	}
	
}
void CMemory::zeroPageWrite(Address location, Byte data)
{
	switch (location)
	{
	case 0xFF00:
		ZRAM[0x00] = (data & 0x30);
		break;
	case 0xFF04:
		ZRAM[0x04] = 0;
		break;
	case 0xFF41:
		ZRAM[0x41] = (data & 0xFC) | (STAT.get() & 0x03);
		break;
	case 0xFF44:
		ZRAM[0x44] = 0;
		break;

	case 0xFF46:
		ZRAM[0x46] = data;
		dmaTransfer();
		break;
	default:
		ZRAM[location & 0xFF] = data;
		break;
	}
}