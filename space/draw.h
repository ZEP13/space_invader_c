#ifndef DRAW_H
#define DRAW_H

#include "config.h"
#include "raylib.h"

extern int ship_pixels[11][11];
extern int enemy_pixels[7][7];
extern int enemy_pixels_lvl2[7][7];
extern int enemy_pixels_lvl3[7][7];
extern int enemy_pixels_lvl4[7][7];
extern int protection_shape[PROTECTION_LIFE][3][7];

void draw_pixels(Vector2 pos, int scale, Color color, int rows, int cols,
                 int pixels[rows][cols]);
#endif
