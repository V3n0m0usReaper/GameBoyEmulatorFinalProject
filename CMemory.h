#pragma once

#include "CDataTypes.h"
#include "Cmc.h"
class CMemory
{
private:
	Cmc* controller;
	void dmaTransfer();
	Byte joypadState();

public:
	vector<Byte> VRAM;
	vector<Byte> OAM;
	vector<Byte> WRAM;
	vector<Byte> ZRAM;



	MemoryRegister P1; // used for joypad input
	MemoryRegister DIV, TIMA, TMA, TAC; // these are the timer and divider registers
	MemoryRegister LCDC, STAT, SCY, SCX, LYC, LY, DMA;// these registers are used to make sure that the screen is displaying the correct things.
	MemoryRegister WY, WX;// these track the position of the window that the gameboy will run off
	MemoryRegister BGP, OBP0, OBP1;// These registers are used to keep track of the colour palette of the game making sure that the sprites and screen are coloured correctly
	MemoryRegister IF, IE;// these are the interupt flag and the enable for the  interupt.

	Byte vMode;
	Byte jpButtons;
	Byte jpArrows;

	string rom;

	CMemory();
	void reset();
	void romLoad(string fileLocation);

	Byte readData(Address addressLocation);

	void vWrite(ofstream& file, vector<Byte>& vector);
	void vLoad(ifstream& file, vector<Byte>& vector);
	void save(ofstream& file);
	void load(ifstream& file);

	void writeData(Address addressLocation, Byte data);
	void zeroPageWrite(Address addressLocation, Byte data);
};

