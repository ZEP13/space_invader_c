#include "bullet.h"
#include "config.h"
#include "draw.h"
#include "enemy.h"
#include "game.h"
#include "raylib.h"
#include "ship.h"

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
  SetTargetFPS(60);

  Player player;
  Game game;
  restart_game(&game);
  player_init(&player);
  init_enemies(game.enemies);

  while (!WindowShouldClose()) {
    if (game.state == GAME_PLAYING) {

      deplacer_vaisseau(&player.position, player.speed, player.width,
                        player.height, SCREEN_WIDTH, SCREEN_HEIGHT);

      shoot_bullet(player, game.bullets);
      update_bullets(game.bullets);
      update_enemies(game.enemies, game.bullets, &game.enemyShootTimer,
                     &game.lastActiveRow);
      handle_collisions(game.bullets, game.enemies, &game.score,
                        game.lastActiveRow);
      handle_collisions_player(game.bullets, &player, &game.state);

      BeginDrawing();
      ClearBackground(BLACK);

      draw_pixels(player.position, 3, BLUE, 7, 7, ship_pixels);
      draw_enemies(game.enemies);
      draw_bullets(game.bullets);

      DrawText(TextFormat("Score: %d", game.score), 10, 10, 20, WHITE);
      DrawText(TextFormat("Lifes: %d", player.player_lifes), 10, 40, 20, WHITE);
    } else if (game.state == GAME_OVER) {
      DrawText("GAME OVER", 300, 200, 40, RED);
      DrawText(TextFormat("Score: %d", game.score), 320, 260, 20, WHITE);
      DrawText("Press ENTER to restart", 250, 320, 20, GRAY);

      if (IsKeyPressed(KEY_ENTER)) {
        restart_game(&game);
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
