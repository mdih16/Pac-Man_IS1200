#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"
#include "graphics.h"
#include "peripherals.h"
#include "game_entities.h"

void user_isr(void);

pacman pac = {56, 20, 56, 20, 3, 0, UP, UP, pacman_full_open_right, 0};
ghost blinky = {4, 4, 56, 8, 0, 0, LEFT, ghost_sprite, BLINKY};
ghost pinky = {16, 4, 56, 8, 0, 0, RIGHT, ghost_sprite, PINKY};
ghost inky = {68, 4, 56, 8, 0, 0, RIGHT, ghost_sprite, INKY};
ghost clyde = {72, 4, 56, 8, 0, 0, RIGHT, ghost_sprite, CLYDE};

int main() {
	display_host_init();
	display_init();
	BTN_init();
	LED_init();
	TMR2_init();
	init_map();

	int i;
	for (i = 0; i < DISPLAY_BUFFER_SIZE; i++)
	{
		display_buffer[i] = 0x00;
	}

	while(1)
	{
	}
	return 0;
}

void user_isr(void)
{
	IFS(0) &= 0xfffffeff;

	update_pacman(&pac);
	//update_ghost(&blinky, &pac);
	//update_ghost(&pinky, &pac);
	//update_ghost(&inky, &pac);
	//update_ghost(&clyde, &pac);
	//ghost_collision(&pac, &blinky);
	//update_map();
	update_tiles(pac.current_tile);
	encode_framebuffer(map);
	display_update();
}

