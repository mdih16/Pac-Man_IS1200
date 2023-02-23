#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"
#include "graphics.h"
#include "peripherals.h"
#include "game_entities.h"

int score = 0;

const uint8_t empty_tile[4] = {0x0, 0x0, 0x0, 0x0};
const uint8_t pellet_tile[4] = {0x0, 0x4, 0x2, 0x0};
const uint8_t power_pellet_tile[4] = {0x0, 0x6, 0x6, 0x0};
const uint8_t wall_horizontal_tile[4] = {0x0, 0xf, 0xf, 0x0};
const uint8_t wall_vertical_tile[4] = {0x6, 0x6, 0x6, 0x6};
const uint8_t wall_top_right_corner_tile[4] = {0x0, 0xe, 0xe, 0x6};
const uint8_t wall_top_left_corner_tile[4] = {0x0, 0x7, 0x7, 0x6};
const uint8_t wall_bottom_right_corner_tile[4] = {0x6, 0xe, 0xe, 0x0};
const uint8_t wall_bottom_left_corner_tile[4] = {0x6, 0x7, 0x7, 0x0};

const uint8_t *tile_sprite[9] = {empty_tile, pellet_tile, power_pellet_tile, wall_horizontal_tile, wall_vertical_tile, wall_top_right_corner_tile, wall_top_left_corner_tile, wall_bottom_right_corner_tile, wall_bottom_left_corner_tile};

const uint8_t ghost_sprite[4] = {0xf, 0x9, 0xf, 0x9};

const uint8_t initial_tiles[256] = {
    6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5,
    4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 8, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4,
    4, 1, 3, 3, 3, 1, 6, 5, 1, 4, 1, 1, 1, 4, 1, 1, 1, 1, 6, 3, 3, 1, 3, 3, 3, 3, 3, 5, 1, 6, 3, 4,
    4, 1, 1, 1, 1, 1, 8, 7, 1, 8, 3, 3, 1, 6, 3, 3, 3, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 4, 2, 4,
    4, 2, 4, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 8, 3, 3, 3, 3, 7, 1, 3, 3, 5, 1, 6, 5, 1, 1, 1, 1, 1, 4,
    4, 3, 7, 1, 8, 3, 3, 3, 3, 3, 1, 3, 3, 3, 1, 1, 1, 1, 4, 1, 1, 1, 4, 1, 8, 7, 1, 3, 3, 3, 1, 4,
    4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 5, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7
    };

uint8_t tiles[256];

void init_map()
{
    uint8_t tile;
    for (tile = 0; tile < 256; tile++)
    {
        uint8_t x = (tile % 32) * 4;
        uint8_t y = (tile / 32) * 4;
        render_object(x, y, TILE_WIDTH, TILE_HEIGHT, tile_sprite[initial_tiles[tile]]);
        tiles[tile] = initial_tiles[tile];
    }
}

uint8_t distance (uint8_t tile1, uint8_t tile2)
{
    int x1 = tile1 % 32;
    int y1 = tile1 / 32;

    int x2 = tile2 % 32;
    int y2 = tile2 / 32;

    return floor_sqrt(power((x2 - x1), 2) + power((y2 - y1), 2));
}

void update_pacman(pacman *pacman)
{
    volatile uint8_t btns = BTN_check();
    if (btns)
    {
        if (btns & 0x1)
            pacman->next_direction = RIGHT;
        if (btns & 0x2)
            pacman->next_direction = DOWN;
        if (btns & 0x4)
            pacman->next_direction = UP;
        if (btns & 0x8)
            pacman->next_direction = LEFT;
    }   

    // Calculate the tile that pacman is currently in
    pacman->current_tile = (((pacman->y + 1) / 4) * 32) + (pacman->x + 2) / 4;

    if (pacman->x % 4 == 0 && pacman->y % 4 == 0)
    {
        if (pacman->next_direction == RIGHT && tiles[pacman->current_tile + 1] < 3)
            pacman->direction = RIGHT;
        else if (pacman->next_direction == DOWN && tiles[pacman->current_tile + 32] < 3)
            pacman->direction = DOWN;
        else if (pacman->next_direction == UP && tiles[pacman->current_tile - 32] < 3)
            pacman->direction = UP;
        else if (pacman->next_direction == LEFT && tiles[pacman->current_tile - 1] < 3)
            pacman->direction = LEFT;
    }

    pacman->x_prev = pacman->x;
    pacman->y_prev = pacman->y;

    if (pacman->direction == RIGHT && tiles[((pacman->y / 4) * 32) + ((pacman->x + 4) / 4)] < 3)
        pacman->x++;
    else if (pacman->direction == DOWN && tiles[(((pacman->y + 4) / 4) * 32) + (pacman->x / 4)] < 3)
        pacman->y++;
    else if (pacman->direction == UP && tiles[(((pacman->y - 1) / 4) * 32) + (pacman->x / 4)] < 3)
        pacman->y--;
    else if (pacman->direction == LEFT && tiles[((pacman->y / 4) * 32) + ((pacman->x - 1) / 4)] < 3)
        pacman->x--;

    render_object(pacman->x_prev, pacman->y_prev, 4, 4, empty);
    // Appearance according to pixel values odd even
    render_object(pacman->x, pacman->y, 4, 4, pacman->appearance);
    tiles[pacman->current_tile] = 0;
}

void update_ghost(ghost *ghost, pacman *pacman)
{
    ghost->current_tile = (((ghost->y + 1) / 4) * 32) + (ghost->x + 2) / 4;
    uint8_t pacman_tile = pacman->current_tile;

    if (ghost->name == BLINKY)
    {
        ghost->target_tile = pacman_tile;
    }
    else if (ghost->name == PINKY)
    {
        if (pacman->direction == RIGHT)
            ghost->target_tile = pacman_tile + 4;
        if (pacman->direction == DOWN)
            ghost->target_tile = pacman_tile + (4 * 32);
        if (pacman->direction == UP)
            ghost->target_tile = pacman_tile - (4 * 32);
        if (pacman->direction == LEFT)
            ghost->target_tile = pacman_tile - 4;
    }
    else if (ghost->name == INKY)
    {
        if (pacman->direction == RIGHT)
            ghost->target_tile = pacman_tile - 4;
        if (pacman->direction == DOWN)
            ghost->target_tile = pacman_tile - (4 * 32);
        if (pacman->direction == UP)
            ghost->target_tile = pacman_tile + (4 * 32);
        if (pacman->direction == LEFT)
            ghost->target_tile = pacman_tile + 4;
    }
    else if (ghost->name == CLYDE)
    {
        if (distance(ghost->current_tile, pacman_tile) < 8)
            ghost->target_tile = 163;
        else
            ghost->target_tile = pacman_tile;
    }

    if (ghost->x % 4 == 0 && ghost->y % 4 == 0)
    {
        uint8_t dist_tile_right = distance((ghost->current_tile + 1), ghost->target_tile);
        uint8_t dist_tile_down = distance((ghost->current_tile + 32), ghost->target_tile);
        uint8_t dist_tile_up = distance((ghost->current_tile - 32), ghost->target_tile);
        uint8_t dist_tile_left = distance((ghost->current_tile - 1), ghost->target_tile);

        if (ghost->direction == RIGHT)
        {
            if (dist_tile_down <= dist_tile_right && dist_tile_down <= dist_tile_up && tiles[(ghost->current_tile + 32)] < 3)
                ghost->direction = DOWN;
            else if (dist_tile_up <= dist_tile_right && dist_tile_up <= dist_tile_down && tiles[(ghost->current_tile - 32)] < 3)
                ghost->direction = UP;
        }
        else if (ghost->direction == DOWN)
        {
            if (dist_tile_right <= dist_tile_down && dist_tile_right <= dist_tile_left && tiles[(ghost->current_tile + 1)] < 3)
                ghost->direction = RIGHT;
            else if (dist_tile_left <= dist_tile_down && dist_tile_left <= dist_tile_right && tiles[(ghost->current_tile - 1)] < 3)
                ghost->direction = LEFT;
        }
        else if (ghost->direction == UP)
        {
            if (dist_tile_right <= dist_tile_up && dist_tile_right <= dist_tile_left && tiles[(ghost->current_tile + 1)] < 3)
                ghost->direction = RIGHT;
            else if (dist_tile_left <= dist_tile_up && dist_tile_left <= dist_tile_right && tiles[(ghost->current_tile - 1)] < 3)
                ghost->direction = LEFT;
        }
        else if (ghost->direction == LEFT)
        {
            if (dist_tile_down <= dist_tile_left && dist_tile_down <= dist_tile_up && tiles[(ghost->current_tile + 32)] < 3)
                ghost->direction = DOWN;
            else if (dist_tile_up <= dist_tile_left && dist_tile_up <= dist_tile_down && tiles[(ghost->current_tile - 32)] < 3)
                ghost->direction = UP;
        }
    }

    ghost->x_prev = ghost->x;
    ghost->y_prev = ghost->y;

    if (ghost->direction == RIGHT)
        ghost->x++;
    else if (pacman->direction == DOWN)
        ghost->y++;
    else if (pacman->direction == UP)
        ghost->y--;
    else if (pacman->direction == LEFT)
        ghost->x--;

    render_object(ghost->x_prev, ghost->y_prev, 4, 4, empty);
    render_object(ghost->x, ghost->y, 4, 4, ghost->appearance);
    //render_object(ghost->target_tile % 32, ghost->target_tile / 8, 4, 4, empty);
}

void ghost_collision (pacman *pacman, ghost *ghost)
{
    if (pacman->current_tile == ghost->current_tile)
    {
        render_object(4,4,4,4,empty);
    }
}

void update_tiles(pacman *pacman)
{
    uint8_t tile = pacman->current_tile;
    uint8_t x = (tile % 32) * 4;
    uint8_t y = (tile / 32) * 4;


    if (tiles[tile] == PELLET)
    {
        tiles[tile] = EMPTY;
        //PORTE += 10;
    }
}