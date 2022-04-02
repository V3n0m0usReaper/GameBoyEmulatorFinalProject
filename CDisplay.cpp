#include "CDisplay.h"

void CDisplay::initialise(CMemory* _memory)
{
	memory = _memory;

	int scale = 5;

	window.create(sf::VideoMode(width, height), "Gameboy Emulator");
	window.setSize(sf::Vector2u(width * scale, height * scale));
	window.setKeyRepeatEnabled(false);

	bArray.create(160, 144, sf::Color(255, 0, 255));
	wArray.create(160, 144, sf::Color(0, 0, 0, 0));
	sArray.create(160, 144, sf::Color(0, 0, 0, 0)); 

	greyShades[0x0] = sf::Color(255, 255, 255); 
	greyShades[0x1] = sf::Color(198, 198, 198); 
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
	bool do_sprites = memory->LCDC.bitIsSet(BIT_1);

	if (do_sprites)
		renderSprite();

	sf::Texture bg_texture;
	sf::Texture window_texture;
	sf::Texture sprites_texture;

	window_texture.loadFromImage(wArray);
	bg_texture.loadFromImage(bArray);
	sprites_texture.loadFromImage(sArray);

	wSprite.setTexture(window_texture);
	bSprite.setTexture(bg_texture);
	sSprite.setTexture(sprites_texture);

	window.draw(bSprite);
	window.draw(wSprite);
	window.draw(sSprite);

	window.display();
}

void CDisplay::clearWindow()
{
	wArray.create(160, 144, sf::Color::Transparent);
}

void CDisplay::updateScanline(Byte current_scanline)
{
	scanlines++;

	bool do_background = memory->LCDC.bitIsSet(BIT_0);
	bool do_window = memory->LCDC.bitIsSet(BIT_5);

	if (do_background)
		updateBackgroundScanline(current_scanline);

	if (do_window)
		updateWindowScanline(current_scanline);
}

void CDisplay::updateBackgroundScanline(Byte current_scanline)
{
	bool bg_code_area = memory->LCDC.bitIsSet(BIT_3);

	if (enableDebug)
	{
		bg_code_area = forceBgMap;
	}

	Address tile_map_location = (bg_code_area) ? 0x9C00 : 0x9800;
	Byte scroll_x = memory->SCX.get();
	Byte scroll_y = memory->SCY.get();

	Byte palette = memory->BGP.get();

	int y = current_scanline;

	for (int x = 0; x < 160; x++)
	{
		int map_x = (int)scroll_x + x;
		int map_y = (int)scroll_y + y;

		map_x = (map_x >= 256) ? map_x - 256 : map_x;
		map_y = (map_y >= 256) ? map_y - 256 : map_y;

		int tile_col = floor(map_x / 8);
		int tile_row = floor(map_y / 8);
		int tile_map_id = (tile_row * 32) + tile_col;
		Address loc = tile_map_location + tile_map_id;
		Byte tile_id = memory->readData(loc);

		int tile_x_pixel = map_x % 8;
		int tile_y_pixel = map_y % 8;

		tile_x_pixel = abs(tile_x_pixel - 7);

		updateBackgroundTilePixel(palette, x, y, tile_x_pixel, tile_y_pixel, tile_id);
	}
}

void CDisplay::updateWindowScanline(Byte current_scanline)
{
	bool window_code_area = memory->LCDC.bitIsSet(BIT_6);

	Address tile_map_location = (window_code_area) ? 0x9C00 : 0x9800;

	int window_x = (int)memory->WX.get();
	int window_y = (int)memory->WY.get();

	Byte palette = memory->BGP.get();

	int y = (int)current_scanline;

	for (int x = 0; x < 160; x++)
	{
		int display_x = x + window_x - 7;
		int display_y = y;

		int tile_col = floor(x / 8);
		int tile_row = floor((y - window_y) / 8);
		int tile_map_id = (tile_row * 32) + tile_col;
		Address loc = tile_map_location + tile_map_id;
		Byte tile_id = memory->readData(loc);

		int tile_x_pixel = x % 8;
		int tile_y_pixel = y % 8;

		tile_x_pixel = abs(tile_x_pixel - 7);

		if (current_scanline == 128)
			bool breakpoint = true;

		if (current_scanline < window_y)
		{
			wArray.setPixel(x, y, sf::Color::Transparent);
		}
		else
		{
			updateWindowTilePixel(palette, display_x, display_y, tile_x_pixel, tile_y_pixel, tile_id);
		}
	}
}

void CDisplay::updateBackgroundTilePixel(Byte palette, int display_x, int display_y, int tile_x, int tile_y, Byte tile_id)
{
	bool bg_char_selection = memory->LCDC.bitIsSet(BIT_4);

	if (enableDebug)
	{
		bg_char_selection = forceBgLocation;
	}

	Address bg_data_location = (bg_char_selection) ? 0x8000 : 0x9000;
	Address offset;

	if (bg_char_selection)
	{
		offset = (tile_id * 16) + bg_data_location;
	}
	else
	{
		signedByte direction = (signedByte)tile_id;
		Byte16Bit temp_offset = (bg_data_location)+(direction * 16);
		offset = (Address)temp_offset;
	}

	Byte
		high = memory->readData(offset + (tile_y * 2) + 1),
		low = memory->readData(offset + (tile_y * 2));

	sf::Color color = pixelColorGet(palette, low, high, tile_x, false);
	bArray.setPixel(display_x, display_y, color);
}

void CDisplay::updateWindowTilePixel(Byte palette, int display_x, int display_y, int tile_x, int tile_y, Byte tile_id)
{
	if (display_x >= 160 || display_x < 0)
		return;
	if (display_y >= 144 || display_y < 0)
		return;

	bool bg_char_selection = memory->LCDC.bitIsSet(BIT_4);

	if (enableDebug)
	{
		bg_char_selection = forceBgLocation;
	}

	Address bg_data_location = (bg_char_selection) ? 0x8000 : 0x9000;
	Address offset;

	if (bg_char_selection)
	{
		offset = (tile_id * 16) + bg_data_location;
	}
	else
	{
		signedByte direction = (signedByte)tile_id;
		Byte16Bit temp_offset = (bg_data_location)+(direction * 16);
		offset = (Address)temp_offset;
	}

	Byte
		high = memory->readData(offset + (tile_y * 2) + 1),
		low = memory->readData(offset + (tile_y * 2));

	sf::Color color = pixelColorGet(palette, low, high, tile_x, false);

	wArray.setPixel(display_x, display_y, color);
}

void CDisplay::renderSprite()
{
	Address sprite_data_location = 0xFE00;
	Byte palette_0 = memory->OBP0.get();
	Byte palette_1 = memory->OBP1.get();

	bool use_8x16_sprites = memory->LCDC.bitIsSet(BIT_2);

	for (int sprite_id = 39; sprite_id >= 0; sprite_id--)
	{
		Address offset = sprite_data_location + (sprite_id * 4);
		int y_pos = ((int)memory->readData(offset)) - 16;
		int x_pos = ((int)memory->readData(offset + 1)) - 8;

		Byte tile_id = memory->readData(offset + 2);
		Byte flags = memory->readData(offset + 3);

		bool use_palette_1 = bitSet(flags, BIT_4);
		Byte sprite_palette = (use_palette_1) ? palette_1 : palette_0;

		if (use_8x16_sprites)
		{
			tile_id = tile_id & 0xFE;
			Byte tile_id_bottom = tile_id | 0x01;
			renderSpriteTile(sprite_palette, x_pos, y_pos, tile_id, flags);
			renderSpriteTile(sprite_palette, x_pos, y_pos + 8, tile_id_bottom, flags);
		}
		else
		{
			renderSpriteTile(sprite_palette, x_pos, y_pos, tile_id, flags);
		}
	}
}

void CDisplay::renderSpriteTile(Byte palette, int start_x, int start_y, Byte tile_id, Byte flags)
{
	Address sprite_data_location = 0x8000;

	bool priority = bitSet(flags, BIT_7);
	bool mirror_y = bitSet(flags, BIT_6);
	bool mirror_x = bitSet(flags, BIT_5);

	bool hide_sprite = bitSet(flags, BIT_7);

	for (int y = 0; y < 8; y++)
	{
		int offset = (tile_id * 16) + sprite_data_location;

		Byte
			high = memory->readData(offset + (y * 2) + 1),
			low = memory->readData(offset + (y * 2));

		for (int x = 0; x < 8; x++)
		{
			int pixel_x = (mirror_x) ? (start_x + x) : (start_x + 7 - x);
			int pixel_y = (mirror_y) ? (start_y + 7 - y) : (start_y + y);

			sf::Vector2u bounds = sArray.getSize();

			if (pixel_x < 0 || pixel_x >= bounds.x)
				continue;
			if (pixel_y < 0 || pixel_y >= bounds.y)
				continue;

			sf::Color color = pixelColorGet(palette, low, high, x, true);

			sf::Color bg_color = bArray.getPixel(pixel_x, pixel_y);

			if (priority)
			{
				if (bg_color != greyShades[0x0])
				{
					continue;
				}
			}

			sArray.setPixel(pixel_x, pixel_y, color);
		}
	}
}
sf::Color CDisplay::pixelColorGet(Byte palette, Byte top, Byte bottom, int bit, bool is_sprite)
{
	Byte color_3_shade = (palette >> 6);
	Byte color_2_shade = (palette >> 4) & 0x03; 
	Byte color_1_shade = (palette >> 2) & 0x03; 
	Byte color_0_shade = (palette & 0x03);

	Byte first = (Byte)bitSet(top, bit);
	Byte second = (Byte)bitSet(bottom, bit);
	Byte color_code = (second << 1) | first;

	sf::Color result;

	switch (color_code)
	{
	case 0x0: return (is_sprite) ? sf::Color::Transparent : greyShades[color_0_shade];
	case 0x1: return greyShades[color_1_shade];
	case 0x2: return greyShades[color_2_shade];
	case 0x3: return greyShades[color_3_shade];
	default:  return sf::Color(255, 0, 255);
	}
}

bool CDisplay::checkLCD()
{
	return memory->LCDC.bitIsSet(BIT_7);
}