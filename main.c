#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"
#include "graphics.h"
#include "peripherals.h"
#include "menu.h"
#include "game_entities.h"

void user_isr(void);
int game_state = 0;

int main() {
	display_host_init();
	display_init();
	BTN_init();
	LED_init();
	TMR2_init();

	IEC(0) |= 0x80;
	IPC(1) |= 0x1f000000; 

	IEC(0) |= 0x800;
	IPC(2) |= 0x1f000000; 

	IEC(0) |= 0x8000;
	IPC(3) |= 0x1f000000; 


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

	if (game_state == 0)
	{
		main_menu();
	}
	else if (game_state == 1)
	{
		render_tiles();
		update_pacman(&pac);
		update_game();
	}
	else if (game_state == 2)
	{
		render_scoreboard();
	}
	else if (game_state == 3)
	{
		submit_score();
	}

	encode_framebuffer(map);
	display_update();
}

