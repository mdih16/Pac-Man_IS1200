#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H


typedef enum direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN,
} direction;

// Define states for tiles
#define EMPTY 0
#define WALL 1
#define PELLET 2
#define PACMAN 3
#define GHOST 4

typedef struct entity
{
    uint8_t x, y;
    uint8_t hp;
    uint8_t direction;
    const uint8_t *appearance;
} entity;

void move_entity (entity pacman, uint8_t direction);

#endif