#include "CEmulator.h"


CEmulator::CEmulator()
{
	cpu.initialise(&memory);
	display.initialise(&memory);
}

void CEmulator::run()
{
	sf::Time time;

	while(display.window.isOpen())
	{
		float cyclesPerFrame = cpu.ClockSpeed / framerate;
		float timeBetweenFrames = 1000 / framerate;
		int currentCycle = 0;

		eventHandler();
		while (currentCycle < cyclesPerFrame)
		{
			Opcode code = memory.readData(cpu.PCRegister);

			cpu.opcodeParse(code);
			currentCycle += cpu.cycles;

			updateTimer(cpu.cycles);
			updateScanline(cpu.cycles);
			interrupt();

			cpu.cycles = 0;
		}

		currentCycle = 0;

		int frameTime = time.asMilliseconds();

		float sleepTime = timeBetweenFrames - frameTime;
		if (frameTime < timeBetweenFrames)
		{
			sf::sleep(sf::milliseconds(sleepTime));
		}
		time = time.Zero;
		display.scanlines = 0;
	}
}

void CEmulator::eventHandler()
{
	sf::Event event;

	while (display.window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			display.window.close();
			break;
		case sf::Event::KeyPressed:
			pressed(event.key.code);
			break;
		case sf::Event::KeyReleased:
			released(event.key.code);
			break;

		
		}
	}
}

void CEmulator::pressed(Key key)
{
	if (key >= 85 && key <= 96)
	{
		int id = key - 84;
		if (sf::Keyboard::isKeyPressed(Key::LShift))
		{
			saveState(id);
		}
		else
		{
			loadState(id);
		}
		return;
	}

	if (key == Key::Space)
	{
		cpu.ClockSpeed *= 100;
		return;
	}

	int keyID = getKeyID(key);

	if (keyID < 0)
	{
		return;
	}

	bool directional = false;

	if (key == Key::Up || key == Key::Down || key == Key::Left || key == Key::Right)
	{
		directional = true;
	}

	Byte joypad = (directional) ? memory.jpArrows : memory.jpButtons;
	bool unpressed = bitSet(joypad, keyID);

	if (!unpressed)
	{
		return;
	}
	if (directional)
	{
		memory.jpArrows = clearBit(joypad, keyID);
	}
	else
	{
		memory.jpButtons = clearBit(joypad, keyID);
	}
	requestInterupt(JoypadInterrupt);
}

void CEmulator::released(Key key)
{
	if (key == Key::Space)
	{
		cpu.ClockSpeed /= 100;
	}

	int keyID = getKeyID(key);

	if (keyID < 0)
	{
		return;
	}

	bool directional = false;

	if (key == Key::Up || key == Key::Down || key == Key::Left || key == Key::Right)
	{
		directional = true;
	}

	Byte joypad = (directional) ? memory.jpArrows : memory.jpButtons;
	bool unpressed = bitSet(joypad, keyID);

	if (unpressed)
	{
		return;
	}

	if (directional)
	{
		memory.jpArrows = setBit(joypad, keyID);
	}
	else
	{
		memory.jpButtons = setBit(joypad, keyID);
	}
}
int CEmulator::getKeyID(Key key)
{
	switch (key)
	{
	case Key::A:
	case Key::Right:
		return BIT_0;
	
	case Key::S:
	case Key::Left:
		return BIT_1;

	case Key::X:
	case Key::Up:
		return BIT_2;

	case Key::Z:
	case Key::Down:
		return BIT_3;

	default:
		return -1;

	}
}

void CEmulator::updateDivider(int cycles)
{
	dividerCounter += cycles;

	if (dividerCounter >= 256)
	{
		dividerCounter = 0;
		memory.DIV.set(memory.DIV.get() + 1);
	}
}

void CEmulator::updateTimer(int cycles)
{

	updateDivider(cycles);

	Byte newFrequency = timerFrequency();

	if (tFrequency != newFrequency)
	{
		setTimerFrequency();
		tFrequency = newFrequency;
	}

	if (enableTimer())
	{
		timerCounter -= cycles;

		if (timerCounter <= 0)
		{
			Byte timerValue = memory.TIMA.get();
			setTimerFrequency();

			if (timerValue == 255)
			{
				memory.TIMA.set(memory.TMA.get());
				requestInterupt(TimerInterrupt);
			}
			else
			{
				memory.TIMA.set(timerValue + 1);
			}
		}
	}
}

bool CEmulator::enableTimer()
{
	return memory.TAC.bitIsSet(BIT_2);
}

Byte CEmulator::timerFrequency()
{
	return (memory.TAC.get() & 0x3);
}

void CEmulator::setTimerFrequency()
{
	Byte frequency = timerFrequency();
	tFrequency = frequency;

	switch (frequency)
	{
	case 0:
		timerCounter = 1024;
		break;
	case 1:
		timerCounter = 16;
		break;
	case 2:
		timerCounter = 64;
		break;
	case 3:
		timerCounter = 256;
		break;
	}
}

void CEmulator::requestInterupt(Byte id)
{
	memory.IF.bitSet(id);
}

void CEmulator::interrupt()
{
	if (memory.IF.get() > 0)
	{
		if (memory.IE.get() > 0)
		{
			if(cpu.cpuHalted)
			{
				cpu.cpuHalted = false;
				cpu.PCRegister += 1;
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if(memory.IF.bitIsSet(i))
			{
				if (memory.IE.bitIsSet(i))
				{
					if (cpu.masterInterupt)
					{
						serviceInterupt(i);
					}
				}
			}
		}

	}
	
}

void CEmulator::serviceInterupt(Byte id)
{
	cpu.masterInterupt = false;
	memory.IF.bitClear(id);

	memory.writeData(--cpu.SPRegister, highByte(cpu.PCRegister));
	memory.writeData(--cpu.SPRegister, lowByte(cpu.PCRegister));

	switch (id)
	{
	case VBlankInterrupt: 
		cpu.PCRegister = 0x40;
		break;
	case LCDCInterrupt:
		cpu.PCRegister = 0x48;
		break;
	case TimerInterrupt:
		cpu.PCRegister = 0x50;
		break;
	case SerailInterrupt:
		cpu.PCRegister = 0x58;
		break;
	case JoypadInterrupt:
		cpu.PCRegister = 0x60;
		break;
	}
}
void CEmulator::setLCD()
{
	Byte status = memory.STAT.get();
	Byte currentLine = memory.LY.get();
	Byte currentMode = status & 0x03;
	Byte mode = 0;

	bool interupt = false;

	if (currentLine >= 144)
	{
		mode = 1;
		status = setBit(status, BIT_0);
		status = clearBit(status, BIT_1);
		interupt = bitSet(status, BIT_4);
	}
	else
	{
		int mode2Threshold = 456 - 80;
		int mode3Threshold = mode2Threshold - 172;
		if (scanlineCounter >= mode2Threshold)
		{
			mode = 2;
			status = setBit(status, BIT_1);
			status = clearBit(status, BIT_0);
			interupt = bitSet(status, BIT_5);
		}
		else if (scanlineCounter >= mode3Threshold)
		{
			mode = 3;
			status = setBit(status, BIT_1);
			status = setBit(status, BIT_0);
		
		}
		else
		{
			mode = 0;

			if (currentMode != mode)
			{
				if (currentLine < 144 && display.scanlines <= 144)
				{
					display.updateScanline(currentLine);
				}

			}
			
			status = clearBit(status, BIT_1);
			status = clearBit(status, BIT_0);
			interupt = bitSet(status, BIT_3);
		}
	}

	if(interupt && (mode != currentMode))
	{
		requestInterupt(LCDCInterrupt);
	}

	if (memory.LY.get() == memory.LYC.get())
	{
		status = setBit(status, BIT_2);

		if (bitSet(status, BIT_6))
		{
			requestInterupt(LCDCInterrupt);
		}
	}

	else
	{
		status = clearBit(status, BIT_2);
	}

	memory.STAT.set(status);
	memory.vMode = mode;
}

void CEmulator::updateScanline(int cycles)
{
	scanlineCounter -= cycles;

	setLCD();

	if (memory.LY.get() > 153)
	{
		memory.LY.clear();
	}

	if(scanlineCounter <= 0)
	{
		Byte currentScanline = memory.LY.get();

		memory.LY.set(++currentScanline);
		scanlineCounter = 456;

		if(currentScanline == 144)
		{
			requestInterupt(VBlankInterrupt);
			if (display.scanlines <= 144)
			{
				display.render();
			}
		}

		else if (currentScanline > 153)
		{
			memory.LY.clear();
		}
	}
}
void CEmulator::saveState(int id)
{
	ofstream file;
	string filename = "GameROM/saves/" + memory.rom + "_" + to_string(id) + ".sav";
	file.open(filename, ios::binary | ios::trunc);

	if(!file.bad())
	{
		cpu.save(file);
		memory.save(file);
		file.close();

		cout << "Wrote save" << id << endl;
	}
}
void CEmulator::loadState(int id)
{
	string filename = "GameROM/saves/" + memory.rom + "_" + to_string(id) + ".sav";
	ifstream file(filename, ios::binary);

	if (file.is_open())
	{
		cpu.load(file);
		memory.load(file);
		file.close();

		cout << "loaded state" << id << endl;
	}
}