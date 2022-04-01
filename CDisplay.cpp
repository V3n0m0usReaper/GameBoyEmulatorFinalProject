#include "CDisplay.h"

void CDisplay::initialise(CMemory* _memory)
{
	memory = _memory;

	int scale = 5;

	window.create(sf::VideoMode(width, length), "Gameboy Emulator");
	window.setSize(sf::Vector2u(width * scale, length * scale));
	window.setKeyRepeatEnabled(false);

	bArray.create(160, 144, sf::Color(255, 255, 255));
	wArray.create(160, 144, sf::Color(0, 0, 0, 0));
	sArray.create(160, 144, sf::Color(0, 0, 0, 0));

	greyShades[0x0] = sf::Color(255, 255, 255);
	greyShades[0x1] = sf::Color(198,198,198);
	greyShades[0x2] = sf::Color(127, 127, 127);
	greyShades[0x3] = sf::Color(0, 0, 0);

}
void CDisplay::render()
{
	if (!checkLCD())
	{
		return;
	}

	window.clear(sf::Color::Transparent);

	sArray.create(160, 144, sf::Color::Transparent);

	bool doSprites = memory->LCDC.bitIsSet(BIT_1);

	if (doSprites)
	{
		renderSprite();
	}

	sf::Texture bTexture;
	sf::Texture sTexture;
	sf::Texture wTexture;

	wTexture.loadFromImage(wArray);
	bTexture.loadFromImage(bArray);
	sTexture.loadFromImage(sArray);

	wSprite.setTexture(wTexture);
	bSprite.setTexture(bTexture);
	sSprite.setTexture(sTexture);

	window.draw(bSprite);
	window.draw(wSprite);
	window.draw(sSprite);

	window.display();
}

void CDisplay::clearWindow()
{
	wArray.create(160, 144, sf::Color::Transparent);
}

void CDisplay::updateScanline(Byte currentScanline)
{
	scanlines++;

	bool doBackground = memory->LCDC.bitIsSet(BIT_0);
	bool doWindow = memory->LCDC.bitIsSet(BIT_5);

	if (doBackground)
	{
		updateBackgroundScanline(currentScanline);
	}
	if(doWindow)
	{
		updateWindowScanline(currentScanline);
	}
}

void CDisplay::updateBackgroundScanline(Byte currentScanline)
{
	bool bCodeArea = memory->LCDC.bitIsSet(BIT_3);

	if(enableDebug)
	{
		bCodeArea = forceBgMap;
	}

	Address tileMapLocation = (bCodeArea) ? 0x9C00 : 0x9800;
	Byte scollX = memory->SCX.get();
	Byte scollY = memory->SCY.get();

	Byte palette = memory->BGP.get();

	int y = currentScanline;

	for (int x = 0; x < 160; x++)
	{
		int mapX = (int)scollX + x;
		int mapY = (int)scollY + y;

		mapX = (mapX >= 256) ? mapX - 256 : mapX;
		mapY = (mapY >= 256) ? mapY - 256 : mapY;

		int tileColumn = floor(mapX / 8);
		int tileRow = floor(mapY / 8);
		int tileMapID = (tileRow * 32) + tileColumn;
		Address loc = tileMapLocation + tileMapID;
		Byte tileID = memory->readData(loc);

		int tileXPixel = mapX % 8;
		int tileYPixel = mapY % 8;

		tileXPixel = abs(tileXPixel - 7);

		updateBackgroundTilePixel(palette, x, y, tileXPixel, tileYPixel, tileID);
	}
}

void CDisplay::updateWindowScanline(Byte currentScanline)
{
	bool wCodeArea = memory->LCDC.bitIsSet(BIT_6);

	Address tileMapLocation = (wCodeArea) ? 0x9C000 : 0x9800;

	int windowX = (int)memory->WX.get();
	int windowY = (int)memory->WY.get();

	Byte palette = memory->BGP.get();

	int y = (int)currentScanline;

	for (int x = 0; x < 160; x++)
	{
		int displayX = x + windowX - 7;
		int displayY = y;

		int tileColumn = floor(x / 8);
		int tileRow = floor((y - windowY) / 8);
		int tileMapID = (tileRow * 32) + tileColumn;
		Address loc = tileMapLocation + tileMapID;
		Byte tileID = memory->readData(loc);

		int tileXPixel = x % 8;
		int tileYPixel = y % 8;

		tileXPixel = abs(tileXPixel - 7);

		if (currentScanline == 128)
		{
			bool breakpoint = true;
		}

		if (currentScanline < windowY)
		{
			wArray.setPixel(x, y, sf::Color::Transparent);
		}
		else
		{
			updateWindowTilePixel(palette, displayX, displayY, tileXPixel, tileYPixel, tileID);
		}
	}
}

void CDisplay::updateBackgroundTilePixel(Byte palette, int displayX, int displayY, int tileX, int tileY, Byte tileID)
{
	bool bCharacterSelection = memory->LCDC.bitIsSet(BIT_4);

	if (enableDebug)
	{
		bCharacterSelection = forceBgLocation;
	}

	Address bDataLocation = (bCharacterSelection) ? 0x8000 : 0x9000;
	Address offset;

	if (bCharacterSelection)
	{
		offset = (tileID * 16) + bDataLocation;
	}

	else
	{
		signedByte direction = (signedByte)tileID;
		Byte16Bit tempOffset = (bDataLocation)+(direction * 16);
		offset = (Address)tempOffset;
	}

	Byte high = memory->readData(offset + (tileY * 2) + 1);
	Byte low = memory->readData(offset + (tileY * 2));

	sf::Color color = pixelColorGet(palette, low, high, tileX, false);
	bArray.setPixel(displayX, displayY, color);
}

void CDisplay::updateWindowTilePixel(Byte palette, int displayX, int displayY, int tileX, int tileY, Byte tileID)
{
	if (displayX >= 160 || displayX < 0)
	{
		return;
	}
	if(displayY >= 144 || displayY < 0)
	{
		return;
	}

	bool bCharacterSelection = memory->LCDC.bitIsSet(BIT_4);

	if (enableDebug)
	{
		bCharacterSelection = forceBgLocation;
	}

	Address bDataLocation = (bCharacterSelection) ? 0x8000 : 0x9000;
	Address offset;

	if (bCharacterSelection)
	{
		offset = (tileID * 16) + bDataLocation;
	}

	else
	{
		signedByte direction = (signedByte)tileID;
		Byte16Bit tempOffset = (bDataLocation)+(direction * 16);
		offset = (Address)tempOffset;
	}

	Byte high = memory->readData(offset + (tileY * 2) + 1);
	Byte low = memory->readData(offset + (tileY * 2));

	sf::Color color = pixelColorGet(palette, low, high, tileX, false);
	wArray.setPixel(displayX, displayY, color);
}

void CDisplay::renderSprite()
{
	Address spriteDataLocation = 0xFE00;
	Byte palette0 = memory->OBP0.get();
	Byte palette1 = memory->OBP1.get();

	bool use8x16Sprites = memory->LCDC.bitIsSet(BIT_2);

	for (int spriteID = 39; spriteID >= 0; spriteID--)
	{
		Address offset = spriteDataLocation + (spriteID * 4);
		int posY = ((int)memory->readData(offset )) - 16;
		int posX = ((int)memory->readData(offset + 1)) - 8;

		Byte tileID = memory->readData(offset + 2);
		Byte flags = memory->readData(offset + 3);

		bool usePalette1 = bitSet(flags, BIT_4);
		Byte spritePalette = (usePalette1) ? palette1 : palette0; 

		if (use8x16Sprites)
		{
			tileID = tileID & 0xFE;
			Byte tileIDBottom = tileID | 0x01;
			renderSpriteTile(spritePalette, posX, posY, tileID, flags);
			renderSpriteTile(spritePalette, posX, posY + 8, tileIDBottom, flags);
		}
		else
		{
			renderSpriteTile(spritePalette, posX, posY, tileID, flags);
		}
	}
}

void CDisplay::renderSpriteTile(Byte palette, int xStart, int yStart, Byte tileID, Byte flags)
{
	Address spriteDataLocation = 0x8000;

	bool prio = bitSet(flags, BIT_7);
	bool mirrorY = bitSet(flags, BIT_6);
	bool mirrorX = bitSet(flags, BIT_5);

	bool hideSprite = bitSet(flags, BIT_7);

	for (int y = 0; y < 8; y++)
	{
		int offset = (tileID * 16) + spriteDataLocation;

		Byte high = memory->readData(offset + (y * 2) + 1);
		Byte low = memory->readData(offset + (y * 2));

		for (int x = 0; x < 8; x++)
		{
			int pixelX = (mirrorX) ? (xStart + x) : (xStart + 7 - x);
			int pixelY = (mirrorY) ? (yStart + 7 - y) : (yStart + y);

			sf::Vector2u bounds = sArray.getSize();

			if (pixelX < 0 || pixelX >= bounds.x)
			{
				continue;
			}
			if (pixelY < 0 || pixelY >= bounds.y)
			{
				continue;
			}

			sf::Color colour = pixelColorGet(palette, low, high, x, true);
			sf::Color bColour = bArray.getPixel(pixelX, pixelY);

			if (prio)
			{
				if (bColour != greyShades[0x0])
				{
					continue;
				}
			}
			sArray.setPixel(pixelX, pixelY, colour);
		}
	}
}

sf::Color CDisplay::pixelColorGet(Byte palette, Byte top, Byte bottom, int bit, bool isSprite)
{
	bool colourShade3 = (palette >> 6);
	bool colourShade2 = (palette >> 4) & 0x03;
	bool colourShade1 = (palette >> 2) & 0x03;
	bool colourShade0 = (palette & 0x03);

	Byte firstByte = (Byte)bitSet(top, bit);
	Byte secondByte = (Byte)bitSet(bottom, bit);
	Byte colourCode = (secondByte << 1) | firstByte;

	sf::Color result;

	switch (colourCode)
	{
	case 0x0:
		return (isSprite) ? sf::Color::Transparent : greyShades[colourShade0];
	case 0x1:
		return greyShades[colourShade1];
	case 0x2:
		return greyShades[colourShade2];
	case 0x3:
		return greyShades[colourShade3];
	default:
		return sf::Color(255, 0, 255);
	}

}

bool CDisplay::checkLCD()
{
	return memory->LCDC.bitIsSet(BIT_7);
}