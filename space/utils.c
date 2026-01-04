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

int enemy_below_alive(Enemy enemies[], int row, int col) {

  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (enemies[i].active && enemies[i].indexCol == col &&
        enemies[i].indexRow > row) {
      return 1;
    }
  }
  return 0;
}
