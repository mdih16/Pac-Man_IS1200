#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

// Define tile dimensions
#define TILE_HEIGHT 4
#define TILE_WIDTH 4

// Define directions
#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

// Define states for tiles
#define EMPTY 0
#define PELLET 1 
#define POWER_PELLET 2 
#define WALL_HORTIZONTAL 3
#define WALL_VERTICAL 4
#define WALL_TOP_RIGHT_CORNER 5
#define WALL_TOP_LEFT_CORNER 6 
#define WALL_BOTTOM_RIGHT_CORNER 7
#define WALL_BOTTOM_LEFT_CORNER 8

// Define ghost names
#define BLINKY 0
#define PINKY 1
#define INKY 2
#define CLYDE 3

// Pacman struct
typedef struct pacman
{
    uint8_t x, y;
    uint8_t x_prev, y_prev;
    int hp;
    uint8_t current_tile;
    uint8_t direction;
    uint8_t next_direction;
    const uint8_t *appearance;
} pacman;

// Ghost struct
typedef struct ghost
{
    uint8_t x, y;
    uint8_t current_tile;
    uint8_t target_tile;
    uint8_t direction;
    const uint8_t *appearance;
    uint8_t name;
    unsigned int  prison;
} ghost;

void update_pacman(pacman *pacman);
void update_ghost(ghost *ghost, pacman *pacman);
void ghost_collision(pacman *pacman, ghost *ghost);
void reset_game();
void update_game();
void render_tiles();
void reset_game();

pacman pac;
ghost ghosts[4];

int score;
uint8_t prison_time;
uint8_t fright_time; 
uint8_t scatter_time;

#endif