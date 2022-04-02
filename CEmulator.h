#pragma once

#include <fstream>
#include <SFML/System.hpp>
#include "CCpu.h"
#include "CMemory.h"
#include "CDisplay.h"

typedef sf::Keyboard::Key Key;
class CEmulator
{
public:
	CEmulator();
	void run();
	CCpu cpu;
	CMemory memory;
	CDisplay display;

private:
	float framerate = 60;

	void eventHandler();

	void pressed(Key key);
	void released(Key key);
	int getKeyID(Key key);

	void saveState(int id);
	void loadState(int id);

	int dividerCounter = 0;
	int dividerFrequency = 16384;
	void updateDivider(int cycles);
	int timerCounter = 0;
	Byte tFrequency = 0;
	void updateTimer(int cycles);
	bool enableTimer();
	Byte timerFrequency();
	void setTimerFrequency();

	void requestInterupt(Byte id);
	void interrupt();
	void serviceInterupt(Byte id);

	int scanlineCounter = 456;
	void setLCD();
	void updateScanline(int cycles);
};

