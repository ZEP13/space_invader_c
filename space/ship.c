#include "ship.h"
#include "bullet.h"
#include "config.h"
#include "raylib.h"
#include "utils.h"

void deplacer_vaisseau(Vector2 *pos, float speed, int ship_width,
                       int ship_height, int screenWidth, int screenHeight) {
  if (IsKeyDown(KEY_RIGHT))
    pos->x += speed;
  if (IsKeyDown(KEY_LEFT))
    pos->x -= speed;
  if (IsKeyDown(KEY_UP))
    pos->y -= speed;
  if (IsKeyDown(KEY_DOWN))
    pos->y += speed;

  pos->x = Clamp(pos->x, 0, screenWidth - ship_width);
  pos->y = Clamp(pos->y, 0, screenHeight - ship_height);
}

void player_init(Player *player) {
  *player = (Player){.position = {CENTER_X, SCREEN_HEIGHT - 60},
                     .speed = 4,
                     .width = SHIP_MATRIX_COL * SHIP_SCALE,
                     .height = SHIP_MATRIX_ROW * SHIP_SCALE,
                     .player_lifes = PLAYER_LIFES};
}

void shoot_bullet(Player player, Bullet *bullet) {
  static int ship_timer = 0;
  int dt_ms = GetFrameTime() * 1000;
  ship_timer += dt_ms;

  if (IsKeyPressed(KEY_SPACE) && ship_timer > SHIP_BULLET_COOLDOWN) {
    spawn_bullet(
        bullet,
        (Vector2){player.position.x - 4 + player.width / 2, player.position.y},
        PLAYER_BULLET, PLAYER_BULLET_SPEED);
    ship_timer = 0;
  }
}
