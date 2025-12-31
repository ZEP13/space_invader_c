#include "game.h"
#include "bullet.h"
#include "config.h"
#include "enemy.h"
#include "raylib.h"
#include "ship.h"
#include "utils.h"

void handle_collisions(Bullet bullets[], Enemy enemies[], int *score) {
  for (int b = 0; b < MAX_BULLETS; b++) {
    if (!bullets[b].active || bullets[b].type != PLAYER_BULLET)
      continue;

    Rectangle br = {bullets[b].position.x, bullets[b].position.y, 2, 8};

    for (int e = 0; e < MAX_ENEMIES; e++) {
      if (!enemies[e].active || !enemies[e].killable)
        continue;

      Rectangle er = rect_from_pos(enemies[e].position, enemies[e].width,
                                   enemies[e].height);

      if (CheckCollisionRecs(br, er)) {
        bullets[b].active = 0;
        enemies[e].active = 0;
        (*score) += 100;
        break;
      }
    }
  }
}

void handle_collisions_player(Bullet bullets[], Player *player,
                              GameState *game_state) {
  for (int b = 0; b < MAX_BULLETS; b++) {
    if (!bullets[b].active)
      continue;

    Rectangle br = {bullets[b].position.x, bullets[b].position.y, 2, 8};

    if (bullets[b].active && bullets[b].type == ENEMY_BULLET) {
      Rectangle er =
          rect_from_pos(player->position, player->width, player->height);

      if (CheckCollisionRecs(br, er)) {
        bullets[b].active = 0;
        player->player_lifes -= 1;
        if (player->player_lifes == 0)
          *game_state = GAME_OVER;
      }
    }
  }
}

void restart_game(Game *game) {
  game->score = 0;
  game->enemyShootTimer = 0.0f;
  game->state = GAME_PLAYING;
  game->player.player_lifes = 3;
  game->player.position = (Vector2){(SCREEN_WIDTH - game->player.width) / 2,
                                    SCREEN_HEIGHT - game->player.height - 10};

  for (int i = 0; i < MAX_ENEMIES; i++) {
    game->enemies[i].active = 1;
    game->enemies[i].position = (Vector2){100 + i * 100, 40};
    game->enemies[i].phase = i;
  }

  for (int i = 0; i < MAX_BULLETS; i++) {
    game->bullets[i].active = 0;
  }
}
