#include "enemy.h"
#include "bullet.h"
#include "config.h"
#include "draw.h"
#include "raylib.h"
#include <math.h>

void init_enemies(Enemy enemies[]) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    enemies[i] = (Enemy){.position = {100 + i * 100, 40},
                         .speed = 1.5f,
                         .width = 18,
                         .height = 18,
                         .active = 1,
                         .phase = i};
  }
}

void update_enemies(Enemy enemies[], Bullet bullets[], float *shootTimer) {
  *shootTimer += GetFrameTime();

  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemies[i].active)
      continue;

    enemies[i].phase += 0.05f;
    enemies[i].position.x += sinf(enemies[i].phase) * 1.5f;
    enemies[i].position.y += enemies[i].speed * 0.02f;

    if (*shootTimer > 1.2f) {
      spawn_bullet(bullets,
                   (Vector2){enemies[i].position.x + enemies[i].width / 2,
                             enemies[i].position.y + enemies[i].height},
                   ENEMY_BULLET);
    }
  }

  if (*shootTimer > 1.2f)
    *shootTimer = 0;
}

void draw_enemies(Enemy enemies[]) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemies[i].active)
      continue;
    draw_pixels(enemies[i].position, 6, VIOLET, 3, 3, enemy_pixels);
  }
}
