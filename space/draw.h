#ifndef DRAW_H
#define DRAW_H

#include "config.h"
#include "raylib.h"

extern int ship_pixels[SHIP_MATRIX_ROW][SHIP_MATRIX_COL];
extern int enemy_pixels[7][7];
extern int enemy_pixels_lvl2[7][7];
extern int enemy_pixels_lvl3[7][7];
extern int enemy_pixels_lvl4[7][7];
extern int protection_shape[PROTECTION_LIFE][PROTECTION_ROW][PROTECTION_COL];

void draw_pixels(Vector2 pos, int scale, Color color, int rows, int cols,
                 int pixels[rows][cols]);
#endif
