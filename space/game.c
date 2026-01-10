#include "game.h"
#include "bullet.h"
#include "config.h"
#include "enemy.h"
#include "protection.h"
#include "raylib.h"
#include "ship.h"
#include "utils.h"
#include <stdio.h>

void handle_collisions(Bullet bullets[], Enemy enemies[], int *score,
                       int lastActiveRow, GameState *game_state) {
  for (int b = 0; b < MAX_BULLETS; b++) {
    if (!bullets[b].active || bullets[b].type != PLAYER_BULLET)
      continue;

    Rectangle br = {bullets[b].position.x - bullets[b].size / 2,
                    bullets[b].position.y, bullets[b].size,
                    bullets[b].size * 2};

    for (int e = 0; e < MAX_ENEMIES; e++) {
      if (!enemies[e].active || enemies[e].indexRow != lastActiveRow)
        continue;

      Rectangle er = rect_from_pos(enemies[e].position, enemies[e].width,
                                   enemies[e].height);

      if (CheckCollisionRecs(br, er)) {
        bullets[b].active = 0;
        enemies[e].active = 0;
        (*score) += enemies[e].killValue;
        int anyActive = 0;
        for (int i = 0; i < MAX_ENEMIES; i++) {
          if (enemies[i].active) {
            anyActive = 1;
            break;
          }
        }
        if (!anyActive) {
          *game_state = GAME_VICTORY;
          save_best_score(*score);
        }
        break;
      }
    }
  }
}

void handle_collisions_player(Bullet bullets[], Player *player,
                              GameState *game_state, int *score) {
  for (int b = 0; b < MAX_BULLETS; b++) {
    if (!bullets[b].active)
      continue;

    Rectangle br = {bullets[b].position.x - bullets[b].size / 2,
                    bullets[b].position.y, bullets[b].size,
                    bullets[b].size * 2};

    if (bullets[b].active && bullets[b].type == ENEMY_BULLET) {
      Rectangle er =
          rect_from_pos(player->position, player->width, player->height);

      if (CheckCollisionRecs(br, er)) {
        bullets[b].active = 0;
        player->player_lifes -= 1;
        if (player->player_lifes == 0) {
          *game_state = GAME_OVER;
          save_best_score(*score);
        }
      }
    }
  }
}

void handle_protection_collision(Bullet bullets[], Protection protections[]) {
  for (int i = 0; i < MAX_PROTECTION; i++) {
    if (protections[i].life <= 0)
      continue;

    Rectangle pr = rect_from_pos(protections[i].position, protections[i].width,
                                 protections[i].height);

    for (int j = 0; j < MAX_BULLETS; j++) {
      if (!bullets[j].active)
        continue;

      Rectangle br = {bullets[j].position.x - bullets[j].size / 2,
                      bullets[j].position.y, bullets[j].size,
                      bullets[j].size * 2};

      if (CheckCollisionRecs(pr, br)) {
        bullets[j].active = 0;
        if (bullets[j].type != PLAYER_BULLET) {
          protections[i].life--;
          if (protections[i].life < 0)
            protections[i].life = 0;
        }
        break;
      }
    }
  }
}

void enemy_arrive_to_ship(Enemy enemies[], Player player,
                          GameState *game_state) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemies[i].active)
      continue;

    float enemyBottom = enemies[i].position.y + enemies[i].height;
    float playerTop = player.position.y;

    if (enemyBottom >= playerTop) {
      *game_state = GAME_OVER;
      return;
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
  init_protection(game->protections);
  init_enemies(game->enemies);

  for (int i = 0; i < MAX_BULLETS; i++) {
    game->bullets[i].active = 0;
  }
}

void save_best_score(int score) {

  int last_best_score = load_best_score();
  if (score >= last_best_score) {
    FILE *file;
    file = fopen("best_score.csv", "w");
    if (!file)
      printf("Could not open file for writing best score.\n");

    fprintf(file, "%d", score);

    fclose(file);
  }
}

int load_best_score() {
  FILE *file;
  file = fopen("best_score.csv", "r");
  if (!file)
    printf("Could not open file for reading best score.\n");

  int best_score;
  fscanf(file, "%d", &best_score);
  fclose(file);
  return best_score;
}
