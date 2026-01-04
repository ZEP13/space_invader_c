#ifndef BULLET_H
#define BULLET_H

#include "config.h"
#include "raylib.h"

typedef struct {
  Vector2 position;
  float speed;
  int size;
  int active;
  int type;
} Bullet;

/* declarations ONLY */
void spawn_bullet(Bullet bullets[], Vector2 pos, int type, int speed);
void update_bullets(Bullet bullets[]);
void draw_bullets(Bullet bullets[]);

static const int ENEMY_BULLET_SPEED_BY_ROW[MAX_ENEMIES_ROWS] = {600, 450, 300,
                                                                180};
#endif
