#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int ship_pixels[7][7] = {{0, 0, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 1, 0},
                         {1, 1, 0, 1, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 0, 1, 0, 0},
                         {0, 0, 1, 0, 1, 0, 0}};

int galaga_pixel[11][11] = {
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0},
    {0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0}, {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1}, {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};

int enemy_ship_pixels[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};

void draw_ship_pixels(Vector2 pos, int scale, Color color, int rows, int cols,
                      int ship[rows][cols]) {
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      if (ship[y][x]) {
        DrawRectangle(pos.x + x * scale, pos.y + y * scale, scale, scale,
                      color);
      }
    }
  }
}
typedef float (*MovePattern)(float x);

typedef struct {
  Vector2 position;
  float speed;
  int width;
  int height;
  int active;
  Color color;
  MovePattern move;
} Ship;

typedef struct {
  const int width;
  const int height;
  const int fps;
} Screen;

typedef struct {
  Vector2 position;
  float speed;
  int size;
  Color color;
  int active;
  int bullet_type; // 0 for player, 1 for enemy
} Bullet;

// float pattern4(float y) { return y; }

float Clamp(float value, int min, int max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

void deplacer_enemy(Ship *enemy, int screenWidth, int screenHeight) {
  if (!enemy->active)
    return;

  enemy->position.y += enemy->speed;

  if (enemy->move != NULL)
    enemy->position.x = enemy->move(enemy->position.y);

  if (enemy->position.x < 0)
    enemy->position.x = 0;
  if (enemy->position.x > screenWidth - enemy->width)
    enemy->position.x = screenWidth - enemy->width;

  if (enemy->position.y > screenHeight)
    enemy->active = 0;
}

void enemy_shoot_bullet(Ship enemy, Bullet *bullet, float *timer, float delay) {
  if (!enemy.active)
    return;

  *timer += GetFrameTime();

  if (*timer >= delay && !bullet->active) {
    bullet->position = (Vector2){enemy.position.x + enemy.width / 2,
                                 enemy.position.y + enemy.height};
    bullet->bullet_type = 1;
    bullet->active = 1;
    *timer = 0.0f;
  }
}

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

void shoot_bullet(Ship ship, Bullet *bullet) {
  if (IsKeyPressed(KEY_SPACE) && !bullet->active) {
    bullet->position =
        (Vector2){ship.position.x + (float)(ship.width / 2), ship.position.y};
    bullet->active = 1;
    bullet->bullet_type = 0;
  }
}

void update_bullet(Bullet *bullet) {
  if (!bullet->active)
    return;

  if (bullet->bullet_type == 0) {
    bullet->position.y -= bullet->speed; // player bullet
  } else {
    bullet->position.y += bullet->speed; // enemy bullet
  }

  if (bullet->position.y < 0 || bullet->position.y > GetScreenHeight()) {
    bullet->active = 0;
  }
}

void draw_bullet(Bullet bullet) {
  if (!bullet.active)
    return;
  DrawRectangle(bullet.position.x - (float)bullet.size / 2,
                bullet.position.y - bullet.size, bullet.size, bullet.size * 2,
                bullet.color);
}

int main() {
  Screen screen = {800, 500, 60};
  SetTargetFPS(screen.fps);
  InitWindow(screen.width, screen.height, "Space Invader");

  // MovePattern patterns[] = {};
  // int nb_patterns = sizeof(patterns) / sizeof(patterns[0]);

  Ship ship;
  ship.position = (Vector2){(float)screen.width / 2, (float)screen.height / 2};
  ship.speed = 4.0f;
  int scale = 3;
  ship.width = 7 * scale;
  ship.height = 7 * scale;
  ship.color = BLUE;
  int len_ship_ship = sizeof(ship_pixels) / sizeof(ship_pixels[0]);

  Ship enemy;
  enemy.position = (Vector2){screen.width / 2, 0};
  enemy.speed = 3.0f;
  int enemy_scale = 6;
  enemy.width = 7 * enemy_scale;
  enemy.height = 7 * enemy_scale;
  enemy.color = VIOLET;
  int len_enemy_ship = sizeof(enemy_ship_pixels) / sizeof(enemy_ship_pixels[0]);

  Bullet bullet;
  bullet.size = 3;
  bullet.speed = 6;
  bullet.color = RED;
  bullet.active = 0;

  // galaga test
  Ship galaga;
  galaga.position =
      (Vector2){(float)screen.width / 2, (float)screen.height / 3};
  galaga.speed = 4.0f;
  int scale_galaga = 5;
  galaga.width = 7 * scale;
  galaga.height = 7 * scale;
  galaga.color = GREEN;
  int len_galaga_ship = sizeof(galaga_pixel) / sizeof(galaga_pixel[0]);

  float enemy_shoot_timer = 0.0f;
  float enemy_shoot_interval = 2.0f; // Enemy shoots every 2 seconds

  while (!WindowShouldClose()) {

    deplacer_vaisseau(&ship.position, ship.speed, ship.width, ship.height,
                      screen.width, screen.height);

    shoot_bullet(ship, &bullet);
    enemy_shoot_bullet(enemy, &bullet, &enemy_shoot_timer,
                       enemy_shoot_interval);
    update_bullet(&bullet);

    BeginDrawing();
    ClearBackground(BLACK);

    draw_ship_pixels(galaga.position, scale_galaga, galaga.color,
                     len_galaga_ship, len_galaga_ship, galaga_pixel);

    draw_ship_pixels(ship.position, scale, ship.color, len_ship_ship,
                     len_ship_ship, ship_pixels);

    draw_ship_pixels(enemy.position, enemy_scale, enemy.color, len_enemy_ship,
                     len_enemy_ship, enemy_ship_pixels);

    deplacer_enemy(&enemy, screen.width, screen.height);
    draw_bullet(bullet);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
