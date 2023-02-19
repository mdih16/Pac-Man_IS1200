#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"
#include "graphics.h"
#include "game_entities.h"

void user_isr(void);

entity pacman = {0, 0, 3, 1, pacman_full_open};

int main() {
	display_host_init();
	display_init();
	BTN_check();
	TMR2_init();

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
	encode_framebuffer(map);
	display_update();
	//move_entity(pacman, RIGHT);

}

