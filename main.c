#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"
#include "peripherals.h"
#include "menu.h"
#include "game_entities.h"

void user_isr(void);

// Variable to control what is shown on screen
int game_state = 0;

int main() {
	// Initialize everything that is needed
	display_host_init();
	display_init();
	BTN_init();
	LED_init();
	TMR2_init(); 
	SW_init();

	// Clear display buffer just in case
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
	// Reset TMR2 flag
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

	encode_framebuffer(framebuffer);
	display_update();
}

