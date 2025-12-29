#include "bullet.h"
#include "config.h"
#include "raylib.h"

void spawn_bullet(Bullet bullets[], Vector2 pos, int type) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[i].active) {
      bullets[i] = (Bullet){
          .position = pos, .speed = 6, .size = 4, .active = 1, .type = type};
      break;
    }
  }
}

void update_bullets(Bullet bullets[]) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[i].active)
      continue;

    bullets[i].position.y +=
        (bullets[i].type == PLAYER_BULLET ? -1 : 1) * bullets[i].speed;

    if (bullets[i].position.y < 0 || bullets[i].position.y > SCREEN_HEIGHT)
      bullets[i].active = 0;
  }
}

void draw_bullets(Bullet bullets[]) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[i].active)
      continue;
    DrawRectangle(bullets[i].position.x - bullets[i].size / 2,
                  bullets[i].position.y, bullets[i].size, bullets[i].size * 2,
                  bullets[i].type == PLAYER_BULLET ? RED : ORANGE);
  }
}
