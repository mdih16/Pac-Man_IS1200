#include <stdint.h>
#include <pic32mx.h>
#include "SSD1306.h"
#include "graphics.h"
#include "peripherals.h"
#include "game_entities.h"

extern int game_state;

int score = 0;
uint8_t pellets_eaten = 0;
uint8_t power_mode = 0;
uint8_t scatter_mode = 0;
int level = 0;
uint8_t prison_count = 0;
uint8_t prison_time;
uint8_t fright_time; 
uint8_t scatter_time;

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

const uint8_t ghost_sprite[4] = {0xf, 0x9, 0xf, 0x9};

pacman pac = {56, 20, 56, 20, 3, 0, UP, UP, pacman_full_open_right};
ghost ghosts[4] = {
	{4, 4, 0, 0, RIGHT, ghost_sprite, BLINKY, 0}, 
	{4, 4, 0, 0, LEFT, ghost_sprite, PINKY, 0}, 
	{4, 4, 0, 0, RIGHT, ghost_sprite, INKY, 0}, 
	{4, 4, 0, 0, LEFT, ghost_sprite, CLYDE, 0}
};

uint8_t initital_tiles[256] = {
    6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 9, 3, 3, 3, 5, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5,
    4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 22, 1, 1, 1, 8, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4,
    4, 1, 20, 3, 19, 1, 6, 5, 1, 21, 1, 1, 1, 21, 1, 1, 1, 1, 6, 3, 19, 1, 20, 3, 3, 3, 3, 5, 1, 6, 3, 12,
    4, 1, 1, 1, 1, 1, 8, 7, 1, 8, 3, 19, 1, 16, 13, 13, 13, 13, 15, 1, 1, 1, 1, 1, 1, 1, 1, 22, 1, 22, 2, 4,
    4, 2, 21, 1, 21, 1, 1, 1, 1, 1, 1, 1, 1, 18, 14, 14, 14, 14, 17, 1, 20, 3, 5, 1, 6, 5, 1, 1, 1, 1, 1, 4,
    11, 3, 7, 1, 8, 3, 3, 3, 3, 19, 1, 20, 3, 7, 1, 1, 1, 1, 22, 1, 1, 1, 22, 1, 8, 7, 1, 20, 3, 19, 1, 4,
    4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 5, 1, 1, 1, 21, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 8, 3, 3, 3, 10, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7
    };

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

void render_tiles()
{
    int tile;
    for (tile = 0; tile < 256; tile++)
    {
        uint8_t x = (tile % 32) * 4;
        uint8_t y = (tile / 32) * 4;
        render_object(x, y, TILE_WIDTH, TILE_HEIGHT, tile_sprite[tiles[tile]]);
    }
}

uint8_t generate_tile()
{
    unsigned int t;
    unsigned int s;
    return (t ^ s) % 256;
}

uint8_t distance (uint8_t tile1, uint8_t tile2)
{
    int x1 = tile1 % 32;
    int y1 = tile1 / 32;

    int x2 = tile2 % 32;
    int y2 = tile2 / 32;

    return floor_sqrt(power((x2 - x1), 2) + power((y2 - y1), 2));
}

void reset_game()
{
    // Reset pacman to starting position
    pac.x = 56;
    pac.y = 20;

    // Reset ghosts to their starting positions
    ghosts[0].x = 56;
    ghosts[0].y = 8;
    ghosts[0].prison = 0;
    ghosts[0].direction = UP;

    ghosts[1].x = 68;
    ghosts[1].y = 14;
    ghosts[1].prison = 48;

    ghosts[2].x = 62;
    ghosts[2].y = 14;

    ghosts[3].x = 56;
    ghosts[3].y = 14;

    if (level == 0)
    {
        ghosts[2].prison = 100000;
        ghosts[3].prison = 100000;
    }
    else if (level == 1)
    {
        ghosts[2].prison = 96;
        ghosts[3].prison = 100000;
    }
    else
    {
        ghosts[2].prison = 96;
        ghosts[3].prison = 144;
    }

    prison_count = 3;
    pellets_eaten = 0;

    // Restore the maze
    int tile;
    for (tile = 0; tile < 256; tile++)
        tiles[tile] = initital_tiles[tile];
}

void completed_level() 
{
    level++;
    if (level == 1) 
    {
        prison_time = 192;
        fright_time = 64;
        scatter_time = 40;
        reset_game();
    }
    else if (level == 2) 
    {
        prison_time = 160;
        fright_time = 48;
        scatter_time = 20;
        reset_game();
    }
    else if (level == 3) 
    {
        prison_time = 96;
        fright_time = 32;
        scatter_time = 0;
        reset_game();
    }
    else if (level == 4)
    {
        display_clear();
        game_state = 3;
    }
}

void imprison(ghost *ghost)
{
    if (prison_count == 0)
    {
        ghost->x = 56;
        ghost->y = 14;
    }
    else if (prison_count == 1)
    {
        ghost->x = 62;
        ghost->y = 14;
    }
    else if (prison_count == 2)
    {
        ghost->x = 68;
        ghost->y = 14;
    }
    else if (prison_count == 3)
    {
        ghost->x = 56;
        ghost->y = 14;
    }
    ghost->appearance = ghost_sprite;
    render_object(ghost->x, ghost->y, 4, 4, ghost->appearance);
    ghost->prison = prison_time;
    prison_count++;
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
}

int is_wall(int tile)
{
    if (tiles[tile] >= 3)
        return 1;
    else
        return 0;
}

void navigate_ghost(ghost *ghost)
{
    // Calculate distances from the tiles around the ghost to the target tile
    uint8_t r_tile = distance((ghost->current_tile + 1), ghost->target_tile);
    uint8_t d_tile = distance((ghost->current_tile + 32), ghost->target_tile);
    uint8_t u_tile = distance((ghost->current_tile - 32), ghost->target_tile);
    uint8_t l_tile = distance((ghost->current_tile - 1), ghost->target_tile);

    uint8_t r_tile_wall = is_wall(ghost->current_tile + 1); 
    uint8_t d_tile_wall = is_wall(ghost->current_tile + 32); 
    uint8_t u_tile_wall = is_wall(ghost->current_tile - 32); 
    uint8_t l_tile_wall = is_wall(ghost->current_tile - 1); 

    if  (ghost->direction == RIGHT)
    {
        if (!r_tile_wall && ((r_tile <= d_tile && r_tile <= u_tile) || (r_tile <= d_tile && u_tile_wall) || (d_tile_wall && r_tile <= u_tile) || (d_tile_wall && u_tile_wall)))
            ghost->direction = RIGHT;
        else if (!d_tile_wall && ((d_tile <= r_tile && d_tile <= u_tile) || (d_tile <= r_tile && u_tile_wall) || (r_tile_wall && d_tile <= u_tile) || (r_tile_wall && u_tile_wall)))
            ghost->direction = DOWN;
        else if (!u_tile_wall && ((u_tile <= d_tile && u_tile <= r_tile) || (u_tile <= d_tile && r_tile_wall) || (d_tile_wall && u_tile <= r_tile) || (d_tile_wall && r_tile_wall)))
            ghost->direction = UP;
        else 
            ghost->direction = LEFT;
    }
    else if  (ghost->direction == DOWN)
    {
        if (!r_tile_wall && ((r_tile <= d_tile && r_tile <= l_tile) || (r_tile <= d_tile && l_tile_wall) || (d_tile_wall && r_tile <= l_tile) || (d_tile_wall && l_tile_wall)))
            ghost->direction = RIGHT;
        else if (!d_tile_wall && ((d_tile <= r_tile && d_tile <= l_tile) || (d_tile <= r_tile && l_tile_wall) || (r_tile_wall && d_tile <= l_tile) || (r_tile_wall && l_tile_wall)))
            ghost->direction = DOWN;
        else if (!l_tile_wall && ((l_tile <= d_tile && l_tile <= r_tile) || (l_tile <= d_tile && r_tile_wall) || (d_tile_wall && l_tile <= r_tile) || (d_tile_wall && r_tile_wall)))
            ghost->direction = LEFT;
        else 
            ghost->direction = UP;
    }
    else if  (ghost->direction == UP)
    {
        if (!r_tile_wall && ((r_tile <= l_tile && r_tile <= u_tile) || (r_tile <= l_tile && u_tile_wall) || (l_tile_wall && r_tile <= u_tile) || (l_tile_wall && u_tile_wall)))
            ghost->direction = RIGHT;
        else if (!l_tile_wall && ((l_tile <= r_tile && l_tile <= u_tile) || (l_tile <= r_tile && u_tile_wall) || (r_tile_wall && l_tile <= u_tile) || (r_tile_wall && u_tile_wall)))
            ghost->direction = LEFT;
        else if (!u_tile_wall && ((u_tile <= l_tile && u_tile <= r_tile) || (u_tile <= l_tile && r_tile_wall) || (l_tile_wall && u_tile <= r_tile) || (d_tile_wall && r_tile_wall)))
            ghost->direction = UP;
        else 
            ghost->direction = DOWN;
    }
    else if  (ghost->direction == LEFT)
    {
        if (!l_tile_wall && ((l_tile <= d_tile && l_tile <= u_tile) || (l_tile <= d_tile && u_tile_wall) || (d_tile_wall && l_tile <= u_tile) || (d_tile_wall && u_tile_wall)))
            ghost->direction = LEFT;
        else if (!d_tile_wall && ((d_tile <= l_tile && d_tile <= u_tile) || (d_tile <= l_tile && u_tile_wall) || (l_tile_wall && d_tile <= u_tile) || (l_tile_wall && u_tile_wall)))
            ghost->direction = DOWN;
        else if (!u_tile_wall && ((u_tile <= d_tile && u_tile <= l_tile) || (u_tile <= d_tile && l_tile_wall) || (d_tile_wall && u_tile <= l_tile) || (d_tile_wall && l_tile_wall)))
            ghost->direction = UP;
        else 
            ghost->direction = RIGHT;
    }
}

void update_ghost(ghost *ghost, pacman *pacman)
{
    ghost->current_tile = (((ghost->y + 1) / 4) * 32) + (ghost->x + 2) / 4;

    if (ghost->prison)
    {
        int time = ghost->prison--;
        if (time == 1)
        {
            ghost->x = 56;
            ghost->y = 8;
            prison_count--;
            ghost->direction = RIGHT;
        }
        render_object(ghost->x, ghost->y, 4, 4, ghost->appearance);
        return;
    }

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
    else if (power_mode)
    {
        ghost->target_tile = generate_tile();
        
        if (power_mode % 2 == 0)
            ghost->appearance = empty_tile;
        else
            ghost->appearance = ghost_sprite;
    }

    if (ghost->x % 4 == 0 && ghost->y % 4 == 0)
    {
        navigate_ghost(ghost);
    }

    uint8_t x_prev = ghost->x;
    uint8_t y_prev = ghost->y;

    if (ghost->direction == RIGHT)
        ghost->x++;
    else if (ghost->direction == DOWN)
        ghost->y++;
    else if (ghost->direction == UP)
        ghost->y--;
    else if (ghost->direction == LEFT)
        ghost->x--;

    render_object(x_prev, y_prev, 4, 4, empty_tile);
    render_object(ghost->x, ghost->y, 4, 4, ghost->appearance);
}

void ghost_collision(pacman *pacman, ghost *ghost)
{
    if (pacman->current_tile == ghost->current_tile)
    {
        if (power_mode)
        {
            imprison(ghost);
            score += 200;
        }
        else
        {
            pacman->hp--;
            if (pacman->hp > 0)
            {
                reset_game();
            }
            else
            {
                game_state = 3;
                display_clear();
            }
        }
    }
}

void update_game()
{
    uint8_t tile = pac.current_tile;
    uint8_t x = (tile % 32) * 4;
    uint8_t y = (tile / 32) * 4;

    if (tiles[tile] == PELLET)
    {
        tiles[tile] = EMPTY;
        pellets_eaten++;
        score += 10;
    }
    else if (tiles[tile] == POWER_PELLET)
    {
        tiles[tile] = EMPTY;
        scatter_mode = 0;
        power_mode = fright_time;
        pellets_eaten++;
        score += 50;
    }

    int ghost;
    for (ghost = 0; ghost < 4; ghost++)
	{
		update_ghost(&ghosts[ghost], &pac);
		ghost_collision(&pac, &ghosts[ghost]);
	} 

    uint8_t hp = pac.hp;
    if (hp == 3)
        PORTE = 0x7;
    else if (hp == 2)
        PORTE = 0x3;
    else if (hp == 1)
        PORTE = 0x1;
    else
        PORTE = 0x0;

    if (pellets_eaten % 30 == 0)
    {
        scatter_mode = scatter_time;
    }

    if (power_mode)
        power_mode--;
    else if (scatter_mode)
        scatter_mode--;

    if (pellets_eaten == 112)
    {
        if (power_mode)
        {
            int ghost;
            for (ghost = 0; ghost < 4; ghost++)
                ghosts[ghost].appearance = ghost_sprite;
        }
        completed_level();
    }
}