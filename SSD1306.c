/* 
Some functions are taken from https://github.com/is1200-example-projects/hello-display
*/

#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"

// Not by me
void delay(int cyc) {
    int i;
    for(i = cyc; i > 0; i--);
}

// Not by me
uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 0x01));
    return SPI2BUF;
}

// Not by me
void display_host_init()
{
    /* Set up peripheral bus clock */
    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;
    
    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;
    
    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;
    
    /* Set up input pins */
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);
    
    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    
    /* Clear SPIROV*/
    SPI2STATCLR &= ~0x40;
    /* Set CKP = 1, MSTEN = 1; */
    SPI2CON |= 0x60;
    
    /* Turn on SPI */
    SPI2CONSET = 0x8000;
}

void display_init()
{
    SSD1306_COMMAND;
    delay(10);
    VDD_EN;
    delay(1000000);
        
    spi_send_recv(SSD1306_DISPLAYOFF);
    SSD1306_RES_CLR;
    delay(10);
    SSD1306_RES_SET;
    delay(10);
        
    spi_send_recv(SSD1306_CHARGEPUMP);
    spi_send_recv(0x14); // Enable charge pump
        
    spi_send_recv(SSD1306_SETPRECHARGE);
    spi_send_recv(0xF1);
        
    VBAT_EN;
    delay(10000000);
        
    spi_send_recv(SSD1306_SETSEGMENTREMAP);
    spi_send_recv(SSD1306_COMSCANDEC);
        
    spi_send_recv(SSD1306_SETCOMPINS);
    spi_send_recv(0x20); // Enable COM Left/Right remap

    spi_send_recv(SSD1306_DISPLAYON);
}

// Update the display with the contents in the display buffer
void display_update()
{
    int page;
    for(page = 0; page < SSD1306_PAGES; page++)
    {
        SSD1306_COMMAND;
        spi_send_recv(SSD1306_PAGEADDR);
        spi_send_recv(page);

        spi_send_recv(SSD1306_SETLOWCOLUMN);
        spi_send_recv(SSD1306_SETHIGHCOLUMN);

        SSD1306_DATA;
        int column;
        for(column = 0; column < SSD1306_COLUMNS; column++)
        {
            spi_send_recv(display_buffer[(page * 128) + column]);
        }
    }
}

// Clear the framebuffer and update the display
void display_clear()
{
	int i;
	for (i = 0; i < DISPLAY_BUFFER_SIZE; i++)
	{
		framebuffer[i] = 0x00;
	}
    display_update();
}

int leftRotate(uint8_t n, uint8_t d)
{
    return (n << d)|(n >> (8 - d));
}


// Take a sprite and write it to the framebuffer
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


			framebuffer[(y * 16 + h * 16) + ((x + w) / 8)] = framebuffer[(y * 16 + h * 16) + ((x + w) / 8)] & ~power(2, (7 - (x_tmp % 8))) | right_rotate((sprite[h + w / 8] & p), shift);
			x_tmp++;
		}
	}
}

// Write frambuffer to the display buffer, in the framebuffer the bytes are horizontal, while the page addressable mode of the SSD1306 requiers the bytes to be vertical. So the function converts the framebuffer to work with the SSD1306.
void encode_framebuffer(uint8_t framebuffer[])
{
    int page;
    for (page = 0; page < 4; page++)
    {
        int column;
        for (column = 0; column < 16; column++)
        {
            int i;
            for (i = 0; i < 8; i++)
            {
                uint8_t byte_vertical = 0;
                int j;
                for (j = 0; j < 8; j++)
                {
                    byte_vertical |= leftRotate((framebuffer[j * 16 + column + page * 128] & power(2, (7 - i))), (9 + i + j) % 8);
                } 
                display_buffer[column * 8 + page * 128 + i] = byte_vertical;
            }
        }
    }
}