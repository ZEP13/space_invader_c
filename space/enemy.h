#ifndef ENEMY_H
#define ENEMY_H
#include "bullet.h"
#include "config.h"
#include "raylib.h"

typedef struct {
  Vector2 position;
  float speed;
  int width, height;
  int active;
  float phase; // for movement pattern
  float shootTimer;
} Enemy;

void init_enemies(Enemy enemies[]);
void update_enemies(Enemy enemies[], Bullet bullets[], float *shootTimer);
void draw_enemies(Enemy enemies[]);

#endif
