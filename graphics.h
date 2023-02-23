#ifndef GRAPHICS_H
#define GRAPHICS_H

const uint8_t pacman_round[4];
const uint8_t pacman_full_open[4];
const uint8_t pacman_half_open[4];
const uint8_t empty[4];

const uint8_t map[512];

void render_object(int x, int y, int width, int height, const uint8_t *sprite);

#endif