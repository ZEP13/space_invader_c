#ifndef DRAW_H
#define DRAW_H

#include "raylib.h"

extern int ship_pixels[7][7];
extern int enemy_pixels[3][3];
extern int enemy_pixels_lvl2[3][3];
extern int enemy_pixels_lvl3[3][3];
extern int enemy_pixels_lvl4[3][3];

void draw_pixels(Vector2 pos, int scale, Color color, int rows, int cols,
                 int pixels[rows][cols]);
#endif
