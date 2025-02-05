#ifndef SSD1306_H
#define SSD1306_H

// Define the SSD1306 display dimensions
#define SSD1306_COLUMNS 128
#define SSD1306_ROWS 32
#define SSD1306_PAGES 4

#define DISPLAY_BUFFER_SIZE 512 

// Toggle data to be interpreted as commmand or input
#define SSD1306_COMMAND (PORTFCLR = 0x10)
#define SSD1306_DATA (PORTFSET = 0x10)

// Reset display
#define SSD1306_RES_CLR (PORTGCLR = 0x200)
#define SSD1306_RES_SET (PORTGSET = 0x200)

// Command codes
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_SETSEGMENTREMAP 0xA1
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP    0xA0
#define SSD1306_CHARGEPUMP 0x8D

// Display 
#define VDD_EN (PORTFCLR = 0x40)
#define VBAT_EN (PORTFCLR = 0x20) 

uint8_t display_buffer[DISPLAY_BUFFER_SIZE];
uint8_t framebuffer[512];

void display_host_init();
void display_init();
void display_update();
void display_clear();
void render_object(int x, int y, int width, int height, const uint8_t *sprite);
void encode_framebuffer();

#endif
