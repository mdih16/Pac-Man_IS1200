#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"

int main() {
	display_host_init();
	display_init();
	
	int i;
	for (i = 0; i < DISPLAY_BUFFER_SIZE; i++)
	{
		display_buffer[i] = 0x00;
	}

	display_buffer[0] = 0xff;


	while(1)
	{
		display_update();
	}
	return 0;
}
