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
  *player = (Player){.position = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60},
                     .speed = 4,
                     .width = 21,
                     .height = 21,
                     .player_lifes = 3};
}

void shoot_bullet(Player player, Bullet *bullet) {
  if (IsKeyPressed(KEY_SPACE)) {
    spawn_bullet(
        bullet,
        (Vector2){player.position.x + player.width / 2, player.position.y},
        PLAYER_BULLET);
  }
}
