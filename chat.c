#include <math.h>
#include <raylib.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 500

#define MAX_BULLETS 64
#define MAX_ENEMIES 6

#define PLAYER_BULLET 0
#define ENEMY_BULLET 1

/* ================= PIXELS ================= */

int ship_pixels[7][7] = {{0, 0, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 1, 0},
                         {1, 1, 0, 1, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 0, 1, 0, 0},
                         {0, 0, 1, 0, 1, 0, 0}};

int enemy_pixels[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};

/* ================= STRUCTS ================= */

typedef struct {
  Vector2 position;
  float speed;
  int width, height;
  int active;
  float phase; // for movement pattern
} Enemy;

typedef struct {
  Vector2 position;
  float speed;
  int size;
  int active;
  int type;
} Bullet;

typedef struct {
  Vector2 position;
  float speed;
  int width, height;
  int player_lifes;
} Player;

typedef enum { GAME_PLAYING, GAME_OVER } GameState;
/* ================= UTILS ================= */

float clamp(float v, float min, float max) {
  if (v < min)
    return min;
  if (v > max)
    return max;
  return v;
}

Rectangle rect_from_pos(Vector2 p, int w, int h) {
  return (Rectangle){p.x, p.y, w, h};
}

/* ================= DRAW ================= */

void draw_pixels(Vector2 pos, int scale, Color color, int rows, int cols,
                 int pixels[rows][cols]) {
  for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++)
      if (pixels[y][x])
        DrawRectangle(pos.x + x * scale, pos.y + y * scale, scale, scale,
                      color);
}

/* ================= BULLETS ================= */

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

/* ================= ENEMIES ================= */

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

void restart_game(Enemy *enemies, Player *player, int *score,
                  GameState *game_state, float *enemyShootTimer) {
  *score = 0;
  *game_state = GAME_PLAYING;
  player->player_lifes = 3;
  player->position = (Vector2){400, 460};
  player->speed = 4;
  player->width = 21;
  player->height = 21;

  init_enemies(enemies);

  *enemyShootTimer = 0;
}

void draw_enemies(Enemy enemies[]) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemies[i].active)
      continue;
    draw_pixels(enemies[i].position, 6, VIOLET, 3, 3, enemy_pixels);
  }
}

/* ================= COLLISIONS ================= */

void handle_collisions(Bullet bullets[], Enemy enemies[], int *score) {
  for (int b = 0; b < MAX_BULLETS; b++) {
    if (!bullets[b].active || bullets[b].type != PLAYER_BULLET)
      continue;

    Rectangle br = {bullets[b].position.x, bullets[b].position.y, 2, 8};

    for (int e = 0; e < MAX_ENEMIES; e++) {
      if (!enemies[e].active)
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
/* ================= MAIN ================= */

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
  SetTargetFPS(60);

  Player player = {.position = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60},
                   .speed = 4,
                   .width = 21,
                   .height = 21,
                   .player_lifes = 3};

  Bullet bullets[MAX_BULLETS] = {0};
  Enemy enemies[MAX_ENEMIES];
  init_enemies(enemies);

  float enemyShootTimer = 0;
  int score = 0;

  GameState game_state = GAME_PLAYING;
  while (!WindowShouldClose()) {

    /* PLAYER */

    if (game_state == GAME_PLAYING) {

      if (IsKeyDown(KEY_LEFT))
        player.position.x -= player.speed;
      if (IsKeyDown(KEY_RIGHT))
        player.position.x += player.speed;

      player.position.x =
          clamp(player.position.x, 0, SCREEN_WIDTH - player.width);

      if (IsKeyPressed(KEY_SPACE)) {
        spawn_bullet(
            bullets,
            (Vector2){player.position.x + player.width / 2, player.position.y},
            PLAYER_BULLET);
      }

      update_bullets(bullets);
      update_enemies(enemies, bullets, &enemyShootTimer);
      handle_collisions(bullets, enemies, &score);
      handle_collisions_player(bullets, &player, &game_state);

      BeginDrawing();
      ClearBackground(BLACK);

      draw_pixels(player.position, 3, BLUE, 7, 7, ship_pixels);
      draw_enemies(enemies);
      draw_bullets(bullets);

      DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
      DrawText(TextFormat("Lifes: %d", player.player_lifes), 10, 40, 20, WHITE);
    } else if (game_state == GAME_OVER) {
      DrawText("GAME OVER", 300, 200, 40, RED);
      DrawText(TextFormat("Score: %d", score), 320, 260, 20, WHITE);
      DrawText("Press ENTER to restart", 250, 320, 20, GRAY);

      if (IsKeyPressed(KEY_ENTER)) {

        restart_game(enemies, &player, &score, &game_state, &enemyShootTimer);
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
