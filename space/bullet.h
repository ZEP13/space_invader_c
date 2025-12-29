#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

#define MAX_BULLETS 64
#define PLAYER_BULLET 0
#define ENEMY_BULLET 1

typedef struct {
  Vector2 position;
  float speed;
  int size;
  int active;
  int type;
} Bullet;

/* declarations ONLY */
void spawn_bullet(Bullet bullets[], Vector2 pos, int type);
void update_bullets(Bullet bullets[]);
void draw_bullets(Bullet bullets[]);

#endif
