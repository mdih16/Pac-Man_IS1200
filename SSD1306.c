/* 
Some functions are taken from https://github.com/is1200-example-projects/hello-display
*/

#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"

void delay(int cyc) {
    int i;
    for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 0x01));
    return SPI2BUF;
}

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

int leftRotate(uint8_t n, unsigned int d)
{
    /* In n>>d, first d bits are 0.
    To put last 3 bits of at
    first, do bitwise or of n>>d
    with n <<(INT_BITS - d) */
    return (n << d)|(n >> (8 - d));
}

void encode_framebuffer(uint8_t framebuffer[])
{
    int page;
    for (page = 0; page < 4; page++)
    {
        int column;
        for (column = 0; column < 16; column++)
        {
            int p;
            int i;
            for (i = 0; i < 8; i++)
            {
                uint8_t byte_vertical = 0;
                if (i == 0)
                    p = 128;
                else if (i == 1)
                    p = 64;
                else if (i == 2)
                    p = 32;
                else if (i == 3)
                    p = 16;
                else if (i == 4)
                    p = 8;
                else if (i == 5)
                    p = 4;
                else if (i == 6)
                    p = 2;
                else if (i == 7)
                    p = 1;
                int j;
                for (j = 0; j < 8; j++)
                {
                    byte_vertical |= leftRotate((framebuffer[j * 16 + column + page * 128] & p), (9 + i + j) % 8);
                } 
                display_buffer[column * 8 + page * 128 + i] = byte_vertical;
            }
        }
    }
}