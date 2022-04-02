#include "CEmulator.h"
#include "CCpu.h"
#include "CDisplay.h"

//this is the main file for the emulator. This is where any roms that I use will be loaded.
int main(int argc, char* args[])
{
	CEmulator emulator;

	//emulator.memory.romLoad("D:\\## Uni\\Year 3\\Gameboy Test ROMs\\cpu_instrs.gb");
	//emulator.memory.romLoad("D:\\## Uni\\Year 3\\Gameboy Test ROMs\\Tetris (World) (Rev 1).gb");
	emulator.memory.romLoad("D:\\## Uni\\Year 3\\Gameboy Test ROMs\\Pokemon Red.gb");
	//emulator.memory.romLoad("D:\\## Uni\\Year 3\\Gameboy Test ROMs\\Puyo_Puyo_Japan_Rev_1_SGB_Enhanced.gb");

	emulator.run();

	return 0;
}