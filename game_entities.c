#include <stdint.h>
#include "SSD1306.h"
#include "graphics.h"
#include "game_entities.h"

tile_state tiles[256];

void move_entity (entity pacman, uint8_t direction)
{
    int i;
    for (i = 0; i < 6; i++) 
    {
        display_buffer[pacman.x + (32 * pacman.y)] = 0x0;
    }

    pacman.x++;

    for (i = 0; i < 6; i++)
        display_buffer[pacman.x + (32 * pacman.y) + i] = pacman.appearance[i];
}



/*
typedef struct ghost
{
    int x,y;
} ghost;
*/
