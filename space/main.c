#include "bullet.h"
#include "config.h"
#include "draw.h"
#include "enemy.h"
#include "game.h"
#include "raylib.h"
#include "ship.h"

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
  SetTargetFPS(FPS);

  Game game;

  restart_game(&game);
  init_enemies(game.enemies);
  player_init(&game.player);

  while (!WindowShouldClose()) {
    if (game.state == GAME_PLAYING) {

      deplacer_vaisseau(&game.player.position, game.player.speed,
                        game.player.width, game.player.height, SCREEN_WIDTH,
                        SCREEN_HEIGHT);

      shoot_bullet(game.player, game.bullets);
      update_bullets(game.bullets);

      update_enemies(game.enemies, game.bullets, &game.enemyShootTimer,
                     &game.lastActiveRow);

      handle_collisions(game.bullets, game.enemies, &game.score,
                        game.lastActiveRow, &game.state);
      handle_collisions_player(game.bullets, &game.player, &game.state,
                               &game.score);
      BeginDrawing();
      ClearBackground(BLACK);

      draw_pixels(game.player.position, 3, BLUE, 11, 11, ship_pixels);
      draw_enemies(game.enemies);
      draw_bullets(game.bullets);

      DrawText(TextFormat("Score: %d", game.score), 10, 10, 20, WHITE);
      DrawText(TextFormat("Lifes: %d", game.player.player_lifes), 10, 40, 20,
               WHITE);

      EndDrawing();

    } else if (game.state == GAME_OVER) {
      int best_score = load_best_score();

      BeginDrawing();
      ClearBackground(BLACK);

      DrawText("GAME OVER", CENTER_X - 100, CENTER_Y - 120, 40, RED);
      DrawText(TextFormat("Score: %d", game.score), CENTER_X - 100,
               CENTER_Y - 80, 20, WHITE);
      DrawText(TextFormat("Best Score: %d", best_score), CENTER_X - 100,
               CENTER_Y - 60, 20, WHITE);
      DrawText("Press ENTER to restart", CENTER_X - 100, CENTER_Y + 30, 20,
               GRAY);

      if (IsKeyPressed(KEY_ENTER)) {
        restart_game(&game);
        player_init(&game.player);
      }

      EndDrawing();

    } else if (game.state == GAME_VICTORY) {
      int best_score = load_best_score();

      BeginDrawing();
      ClearBackground(BLACK);

      DrawText("YOU WIN!", CENTER_X - 100, CENTER_Y - 120, 40, GREEN);
      DrawText(TextFormat("Score: %d", game.score), CENTER_X - 100,
               CENTER_Y - 80, 20, WHITE);
      DrawText(TextFormat("Best Score: %d", best_score), CENTER_X - 100,
               CENTER_Y - 60, 20, WHITE);
      DrawText("Press ENTER to restart", CENTER_X - 100, CENTER_Y + 30, 20,
               GRAY);

      if (IsKeyPressed(KEY_ENTER)) {
        restart_game(&game);
        player_init(&game.player);
      }

      EndDrawing();
    }
  }

  CloseWindow();
  return 0;
}
