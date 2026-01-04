#include "enemy.h"
#include "bullet.h"
#include "config.h"
#include "draw.h"
#include "raylib.h"
#include "utils.h"
#include <math.h>

void init_enemies(Enemy enemies[]) {
  float spacingX = SCREEN_WIDTH / (float)MAX_ENEMIES_COLS;
  float spacingY = (SCREEN_HEIGHT * 0.66f) / (float)MAX_ENEMIES_ROWS;

  int index = 0;
  for (int row = 0; row < MAX_ENEMIES_ROWS; row++) {
    for (int col = 0; col < MAX_ENEMIES_COLS; col++) {
      if (index >= MAX_ENEMIES)
        break;

      enemies[index].width = 42;
      enemies[index].height = 42;
      enemies[index].speed = 1.5f;
      enemies[index].active = 1;
      enemies[index].phase = 0;
      enemies[index].indexRow = row;
      enemies[index].indexCol = col;
      enemies[index].killValue = (MAX_ENEMIES_ROWS - row) * 10;

      enemies[index].baseX =
          spacingX * col + spacingX / 2.0f - enemies[index].width / 2.0f;
      enemies[index].baseY =
          spacingY * row + spacingY / 2.0f - enemies[index].height / 2.0f;

      enemies[index].position =
          (Vector2){enemies[index].baseX, enemies[index].baseY};

      index++;
    }
  }
}

void update_enemies(Enemy enemies[], Bullet bullets[], float *shootTimer,
                    int *lastActiveRow) {

  static float globalPhase = 0.0f;
  globalPhase += 0.03f;

  *shootTimer += GetFrameTime();

  *lastActiveRow = -1;
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (enemies[i].active && enemies[i].indexRow > *lastActiveRow) {
      *lastActiveRow = enemies[i].indexRow;
    }
  }

  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemies[i].active)
      continue;
    enemies[i].position.x = enemies[i].baseX + sinf(globalPhase) * 20.0f;
    enemies[i].position.y = enemies[i].baseY + enemies[i].speed * 0.02f;

    int speed_enemy_bullet = ENEMY_BULLET_SPEED_BY_ROW[enemies[i].indexRow];

    if (!enemy_below_alive(enemies, enemies[i].indexRow, enemies[i].indexCol) &&
        *shootTimer > ENEMY_BULLET_COOLDOWN) {

      int randomShoot = GetRandomValue(0, 1000);
      if (randomShoot < 850)
        spawn_bullet(bullets,
                     (Vector2){enemies[i].position.x + enemies[i].width / 2,
                               enemies[i].position.y + enemies[i].height},
                     ENEMY_BULLET, speed_enemy_bullet);
    }
  }

  if (*shootTimer > ENEMY_BULLET_COOLDOWN)
    *shootTimer = 0;
}

void draw_enemies(Enemy enemies[]) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemies[i].active)
      continue;
    if (enemies[i].indexRow == 0)
      draw_pixels(enemies[i].position, 6, RED, 7, 7, enemy_pixels_lvl4);
    else if (enemies[i].indexRow == 1)
      draw_pixels(enemies[i].position, 6, ORANGE, 7, 7, enemy_pixels_lvl3);
    else if (enemies[i].indexRow == 2)
      draw_pixels(enemies[i].position, 6, YELLOW, 7, 7, enemy_pixels_lvl2);
    else
      draw_pixels(enemies[i].position, 6, VIOLET, 7, 7, enemy_pixels);
  }
}
