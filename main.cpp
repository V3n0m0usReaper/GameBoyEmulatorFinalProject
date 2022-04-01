#include "CEmulator.h"
#include "CCpu.h"
#include "CDisplay.h"


int main(int argc, char* args[])
{
	CEmulator emulator;

	//emulator.memory.romLoad("D:\\## Uni\\Year 3\\Gameboy Test ROMs\\cpu_instrs.gb");
	emulator.memory.romLoad("D:\\## Uni\\Year 3\\Gameboy Test ROMs\\Tetris (World) (Rev 1).gb");
	//emulator.memory.romLoad("D:\\## Uni\\Year 3\\Gameboy Test ROMs\\Pokemon - Red Version (USA, Europe) (SGB Enhanced).gb");

	emulator.run();

	return 0;
}