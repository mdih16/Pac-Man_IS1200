#include <stdint.h>
#include <pic32mx.h>
#include "math.h"
#include "SSD1306.h"
#include "game_entities.h"
	//1111
	//1111
	//0110

	//0111
	//1100
	//1100
	//0111

	//011110
	//111111
	//111100
	//111100
	//111111
	//011110

void render_object (int x, int y, int width, int height, const uint8_t *sprite)
{
	int h;
	int shift = ((x % 8) + width) % 8;
	for (h = 0; h < height; h++)
	{
		int x_tmp = x;
		int w;
		for (w = 0; w < width; w++)
		{
			uint8_t p;

			if (width < 8)
				p = power(2, (width - 1) - w);
			else
				p = power(2, (7 - w));


			map[(y * 16 + h * 16) + ((x + w) / 8)] = map[(y * 16 + h * 16) + ((x + w) / 8)] & ~power(2, (7 - (x_tmp % 8))) | right_rotate((sprite[h + w / 8] & p), shift);
			x_tmp++;
		}
	}
}