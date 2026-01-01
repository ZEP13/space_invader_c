#ifndef GAME_H
#define GAME_H

#include "bullet.h"
#include "config.h"
#include "enemy.h"
#include "raylib.h"
#include "ship.h"

typedef struct {
  Player player;
  Enemy enemies[MAX_ENEMIES];
  Bullet bullets[MAX_BULLETS];
  GameState state;
  int score;
  float enemyShootTimer;
  int lastActiveRow;
  int bestScore;
} Game;

void handle_collisions(Bullet bullets[], Enemy enemies[], int *score,
                       int lastActiveRow, GameState *game_state);
void handle_collisions_player(Bullet bullets[], Player *player,
                              GameState *game_state, int *score);

void save_best_score(int score);
int load_best_score();
void restart_game(Game *game);
void draw_game_over_screen(Game *game);
void draw_game_play_screen(Game *game);
#endif
