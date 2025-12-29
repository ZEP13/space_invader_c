#ifndef UTILS_H
#define UTILS_H
#include "raylib.h"

float Clamp(float value, int min, int max);

Rectangle rect_from_pos(Vector2 p, int w, int h);

#endif
