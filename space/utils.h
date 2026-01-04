#ifndef UTILS_H
#define UTILS_H
#include "draw.h"
#include "enemy.h"
#include "raylib.h"

float Clamp(float value, int min, int max);
int enemy_below_alive(Enemy enemies[], int row, int col);

Rectangle rect_from_pos(Vector2 p, int w, int h);

#endif
