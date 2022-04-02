#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "CMemory.h"

class CDisplay
{
public:
	sf::RenderWindow window;

	sf::Image bArray;
	sf::Image sArray;
	sf::Image wArray;

	sf::Sprite bSprite;
	sf::Sprite sSprite;
	sf::Sprite wSprite;

	int scanlines = 0;
	int width = 160;
	int height = 144;

	bool palleteEmulation = true;

	bool enableDebug = false;
	bool forceBgMap = false;
	bool forceBgLocation = false;

	void initialise(CMemory* _memory);

	void updateScanline(Byte currentScanline);
	void render();
	bool checkLCD();

private: 
	CMemory* memory;

	const Byte WHITE = 0;
	const Byte LIGHTGREY = 1;
	const Byte DARKGREY = 2;
	const Byte BLACK = 3;

	sf::Color greyShades[4];
	sf::Color pixelColorGet(Byte palette, Byte top, Byte bottom, int bit, bool sprite);

	void updateBackgroundScanline(Byte currentScanline);
	void updateWindowScanline(Byte currentScanline);
	void updateBackgroundTilePixel(Byte palette, int XDisplay, int YDisplay, int XTile, int YTile, Byte tileID);
	void updateWindowTilePixel(Byte palette, int XDisplay, int YDisplay, int XTile, int YTile, Byte tileID);
	void clearWindow();
	void renderSprite();
	void renderSpriteTile(Byte palette, int XStart, int YStart, Byte tileID, Byte flags);
};

