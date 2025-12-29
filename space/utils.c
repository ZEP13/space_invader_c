#include "utils.h"
#include "raylib.h"

float Clamp(float value, int min, int max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

Rectangle rect_from_pos(Vector2 p, int w, int h) {
  return (Rectangle){p.x, p.y, w, h};
}
