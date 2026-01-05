#ifndef PROTECTION_H
#define PROTECTION_H

#include <raylib.h>

typedef struct {
  Vector2 position;
  int width, height;
  int life;
} Protection;

void init_protection(Protection protection[]);
void draw_protection(Protection protection[]);
void update_protection(Protection protection[]);

#endif
