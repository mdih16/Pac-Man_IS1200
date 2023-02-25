#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"
#include "graphics.h"
#include "peripherals.h"
#include "game_entities.h"

uint8_t map[512];
int score = 0;
uint8_t power_mode = 0;
uint8_t scatter_mode = 0;

// Textures for the tiles
const uint8_t empty_tile[4] = {0x0, 0x0, 0x0, 0x0};
const uint8_t pellet_tile[4] = {0x0, 0x4, 0x2, 0x0};
const uint8_t power_pellet_tile[4] = {0x0, 0x6, 0x6, 0x0};
const uint8_t wall_horizontal_tile[4] = {0x0, 0xf, 0xf, 0x0};
const uint8_t wall_vertical_tile[4] = {0x6, 0x6, 0x6, 0x6};
const uint8_t wall_top_right_corner_tile[4] = {0x0, 0xe, 0xe, 0x6};
const uint8_t wall_top_left_corner_tile[4] = {0x0, 0x7, 0x7, 0x6};
const uint8_t wall_bottom_right_corner_tile[4] = {0x6, 0xe, 0xe, 0x0};
const uint8_t wall_bottom_left_corner_tile[4] = {0x6, 0x7, 0x7, 0x0};
const uint8_t wall_fork_top_tile[4] = {0x0, 0xf, 0xf, 0x6};
const uint8_t wall_fork_bottom_tile[4] = {0x6, 0xf, 0xf, 0x0};
const uint8_t wall_fork_right_tile[4] = {0x6, 0x7, 0x7, 0x6};
const uint8_t wall_fork_left_tile[4] = {0x6, 0xe, 0xe, 0x6};
const uint8_t wall_prison_top_horizontal_tile[4] = {0xf, 0x0, 0x0, 0x0};
const uint8_t wall_prison_bottom_horizontal_tile[4] = {0x0, 0x0, 0x0, 0xf};
const uint8_t wall_prison_top_right_corner_tile[4] = {0xe, 0x2, 0x2, 0x2};
const uint8_t wall_prison_top_left_corner_tile[4] = {0x7, 0x4, 0x4, 0x4};
const uint8_t wall_prison_bottom_right_tile[4] = {0x2, 0x2, 0x2, 0xe};
const uint8_t wall_prison_bottom_left_tile[4] = {0x4, 0x4, 0x4, 0x7};
const uint8_t wall_horizontal_edge_right[4] = {0x0, 0xe, 0xe, 0x0};
const uint8_t wall_horizontal_edge_left[4] = {0x0, 0x7, 0x7, 0x0};
const uint8_t wall_vertical_edge_up[4] = {0x0, 0x6, 0x6, 0x6};
const uint8_t wall_vertical_edge_down[4] = {0x6, 0x6, 0x6, 0x0};
const uint8_t *tile_sprite[23] = {empty_tile, pellet_tile, power_pellet_tile, wall_horizontal_tile, wall_vertical_tile, wall_top_right_corner_tile, wall_top_left_corner_tile, wall_bottom_right_corner_tile, wall_bottom_left_corner_tile, wall_fork_top_tile, wall_fork_bottom_tile, wall_fork_right_tile, wall_fork_left_tile, wall_prison_top_horizontal_tile, wall_prison_bottom_horizontal_tile, wall_prison_top_right_corner_tile, wall_prison_top_left_corner_tile, wall_prison_bottom_right_tile, wall_prison_bottom_left_tile, wall_horizontal_edge_right, wall_horizontal_edge_left, wall_vertical_edge_up, wall_vertical_edge_down};

const uint8_t pacman_round[4] = {0x6, 0xF, 0xF, 0x6};
const uint8_t pacman_full_open_right[4] = {0x6, 0xc, 0xc, 0x6};
const uint8_t pacman_full_open_down[4] = {0x6, 0xf, 0x9, 0x0};
const uint8_t pacman_full_open_up[4] = {0x0, 0x9, 0xf, 0x6};
const uint8_t pacman_full_open_left[4] = {0x6, 0x3, 0x3, 0x6};
//const uint8_t pacman_half_open[4] = {0x1E, 0x3F, 0x3F, 0x3F, 0x33, 0x12};

const uint8_t ghost_sprite[4] = {0xf, 0x9, 0xf, 0x9};

uint8_t tiles[256] = {
    6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 9, 3, 3, 3, 5, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5,
    4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 22, 1, 1, 1, 8, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4,
    4, 1, 20, 3, 19, 1, 6, 5, 1, 21, 1, 1, 1, 21, 1, 1, 1, 1, 6, 3, 19, 1, 20, 3, 3, 3, 3, 5, 1, 6, 3, 12,
    4, 1, 1, 1, 1, 1, 8, 7, 1, 8, 3, 19, 1, 16, 13, 13, 13, 13, 15, 1, 1, 1, 1, 1, 1, 1, 1, 22, 1, 22, 2, 4,
    4, 2, 21, 1, 21, 1, 1, 1, 1, 1, 1, 1, 1, 18, 14, 14, 14, 14, 17, 1, 20, 3, 5, 1, 6, 5, 1, 1, 1, 1, 1, 4,
    11, 3, 7, 1, 8, 3, 3, 3, 3, 19, 1, 20, 3, 7, 1, 1, 1, 1, 22, 1, 1, 1, 22, 1, 8, 7, 1, 20, 3, 19, 1, 4,
    4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 5, 1, 1, 1, 21, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 8, 3, 3, 3, 10, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7
    };

void init_map()
{
    uint8_t tile;
    for (tile = 0; tile < 256; tile++)
    {
        uint8_t x = (tile % 32) * 4;
        uint8_t y = (tile / 32) * 4;
        render_object(x, y, TILE_WIDTH, TILE_HEIGHT, tile_sprite[tiles[tile]]);
    }
}

void update_map()
{
    uint8_t tile;
    for (tile = 0; tile < 256; tile++)
    {
        uint8_t x = (tile % 32) * 4;
        uint8_t y = (tile / 32) * 4;
        render_object(x, y, TILE_WIDTH, TILE_HEIGHT, tile_sprite[tiles[tile]]);
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
    {
        pacman->x++;
        pacman->appearance = pacman_full_open_right;
    }
    else if (pacman->direction == DOWN && tiles[(((pacman->y + 4) / 4) * 32) + (pacman->x / 4)] < 3)
    {
        pacman->y++;
        pacman->appearance = pacman_full_open_down;
    }
    else if (pacman->direction == UP && tiles[(((pacman->y - 1) / 4) * 32) + (pacman->x / 4)] < 3)
    {
        pacman->y--;
        pacman->appearance = pacman_full_open_up;
    }
    else if (pacman->direction == LEFT && tiles[((pacman->y / 4) * 32) + ((pacman->x - 1) / 4)] < 3)
    {
        pacman->x--;
        pacman->appearance = pacman_full_open_left;
    }

    if (pacman->x % 2 == 1 || pacman->y % 2 == 1)
        pacman->appearance = pacman_round;

    render_object(pacman->x_prev, pacman->y_prev, 4, 4, empty_tile);
    render_object(pacman->x, pacman->y, 4, 4, pacman->appearance);

    uint8_t hp = pacman->hp;
    if (hp == 3)
        PORTE = 0x7;
    else if (hp == 2)
        PORTE = 0x3;
    else if (hp == 1)
        PORTE = 0x1;
    else
        PORTE = 0x0;
}

void update_ghost(ghost *ghost, pacman *pacman)
{
    ghost->current_tile = (((ghost->y + 1) / 4) * 32) + (ghost->x + 2) / 4;

    if (!power_mode && !scatter_mode)
    {
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
                ghost->target_tile = 195;
            else
                ghost->target_tile = pacman_tile;
        }
    }
    else if (scatter_mode)
    {
        if (ghost->name == BLINKY)
            ghost->target_tile = 33;
        else if (ghost->name == PINKY)
            ghost->target_tile = 91;
        else if (ghost->name == INKY)
            ghost->target_tile = 190;
        else if (ghost->name == CLYDE)
            ghost->target_tile = 195;
    }

    if (ghost->x % 4 == 0 && ghost->y % 4 == 0)
    {
        uint8_t dist_tile_right = distance((ghost->current_tile + 1), ghost->target_tile);
        uint8_t dist_tile_down = distance((ghost->current_tile + 32), ghost->target_tile);
        uint8_t dist_tile_up = distance((ghost->current_tile - 32), ghost->target_tile);
        uint8_t dist_tile_left = distance((ghost->current_tile - 1), ghost->target_tile);

        if (ghost->direction == RIGHT)
        {
            if (dist_tile_right <= dist_tile_down && dist_tile_right <= dist_tile_up && tiles[(ghost->current_tile + 1)] < 3)
                ghost->direction = RIGHT;
            else if (dist_tile_down <= dist_tile_up && tiles[(ghost->current_tile + 32)] < 3)
                ghost->direction = DOWN;
            else if (tiles[(ghost->current_tile - 32)] < 3)
                ghost->direction = UP;
            else if (tiles[(ghost->current_tile + 1)] >= 3 && tiles[(ghost->current_tile + 32)] >= 3 && tiles[(ghost->current_tile - 32)] >= 3)  
                ghost->direction = LEFT;
        }
        else if (ghost->direction == DOWN)
        {
            if (dist_tile_down <= dist_tile_right && dist_tile_down <= dist_tile_left && tiles[(ghost->current_tile + 32)] < 3)
                ghost->direction = DOWN;
            else if (dist_tile_right <= dist_tile_left && tiles[(ghost->current_tile + 1)] < 3)
                ghost->direction = RIGHT;
            else if (tiles[(ghost->current_tile - 1)] < 3)
                ghost->direction = LEFT;
            else if (tiles[(ghost->current_tile + 1)] >= 3 && tiles[(ghost->current_tile - 1)] >= 3 && tiles[(ghost->current_tile + 32)] >= 3)  
                ghost->direction = UP;
        }
        else if (ghost->direction == UP)
        {
            if (dist_tile_up <= dist_tile_right && dist_tile_up <= dist_tile_left && tiles[(ghost->current_tile - 32)] < 3)
                ghost->direction = UP;
            else if (dist_tile_right <= dist_tile_left && tiles[(ghost->current_tile + 1)] < 3)
                ghost->direction = RIGHT;
            else if (tiles[(ghost->current_tile - 1)] < 3)
                ghost->direction = LEFT;
            else if (tiles[(ghost->current_tile + 1)] >= 3 && tiles[(ghost->current_tile - 1)] >= 3 && tiles[(ghost->current_tile - 32)] >= 3)  
                ghost->direction = DOWN;
        }
        else if (ghost->direction == LEFT)
        {
            if (dist_tile_left <= dist_tile_down && dist_tile_left <= dist_tile_up && tiles[(ghost->current_tile - 1)] < 3)
                ghost->direction = LEFT;
            else if (dist_tile_down <= dist_tile_up && tiles[(ghost->current_tile + 32)] < 3)
                ghost->direction = DOWN;
            else if (tiles[(ghost->current_tile - 32)] < 3)
                ghost->direction = UP;
            else if (tiles[(ghost->current_tile - 1)] >= 3 && tiles[(ghost->current_tile + 32)] >= 3 && tiles[(ghost->current_tile - 32)] >= 3)  
                ghost->direction = RIGHT;
        }
    }

    ghost->x_prev = ghost->x;
    ghost->y_prev = ghost->y;

    if (ghost->direction == RIGHT)
        ghost->x++;
    else if (ghost->direction == DOWN)
        ghost->y++;
    else if (ghost->direction == UP)
        ghost->y--;
    else if (ghost->direction == LEFT)
        ghost->x--;

    render_object(ghost->x_prev, ghost->y_prev, 4, 4, empty_tile);
    render_object(ghost->x, ghost->y, 4, 4, ghost->appearance);
}

void ghost_collision (pacman *pacman, ghost *ghost)
{
    if (pacman->current_tile == ghost->current_tile)
    {
        pacman->hp--;
    }
}

void update_tiles(uint8_t tile)
{
    uint8_t x = (tile % 32) * 4;
    uint8_t y = (tile / 32) * 4;

    if (tiles[tile] == PELLET)
    {
        tiles[tile] = EMPTY;
        score += 10;
    }
    else if (tiles[tile] == POWER_PELLET)
    {
        tiles[tile] = EMPTY;
        power_mode = 1;
        score += 50;
    }
}