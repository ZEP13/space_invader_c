#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int ship_pixels[7][7] = {{0, 0, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 1, 0},
                         {1, 1, 0, 1, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 0, 1, 0, 0},
                         {0, 0, 1, 0, 1, 0, 0}};

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
} Bullet;

float pattern1(float y) { return 400 + 100 * sin(y * 0.05f); }
float pattern2(float y) { return 400 + 100 * cos(y * 0.05f); }
float pattern3(float y) { return 400 + 50 * sin(y * 0.1f); }
float pattern4(float y) { return 400 + 50 * cos(y * 0.1f); }

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

  // Utiliser le pattern déjà choisi pour cet ennemi
  if (enemy->move != NULL)
    enemy->position.x = enemy->move(enemy->position.y);

  // Clamp X et désactiver si Y hors écran
  if (enemy->position.x < 0)
    enemy->position.x = 0;
  if (enemy->position.x > screenWidth - enemy->width)
    enemy->position.x = screenWidth - enemy->width;

  if (enemy->position.y > screenHeight)
    enemy->active = 0;
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
  }
}

void update_bullet(Bullet *bullet) {
  if (!bullet->active)
    return;

  bullet->position.y -= bullet->speed;

  if (bullet->position.y + bullet->size < 0) {
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

  MovePattern patterns[] = {pattern1, pattern2, pattern3, pattern4};
  int nb_patterns = sizeof(patterns) / sizeof(patterns[0]);

  Ship ship;
  ship.position = (Vector2){(float)screen.width / 2, (float)screen.height / 2};
  ship.speed = 4.0f;
  int scale = 3;
  ship.width = 7 * scale;
  ship.height = 7 * scale;
  ship.color = BLUE;
  int len_ship_ship = sizeof(ship_pixels) / sizeof(ship_pixels[0]);

  Ship enemy;
  enemy.position = (Vector2){(float)screen.width / 2, (float)screen.height / 2};
  enemy.speed = 4.0f;
  int enemy_scale = 6;
  enemy.width = 7 * enemy_scale;
  enemy.height = 7 * enemy_scale;
  enemy.color = VIOLET;
  int len_enemy_ship = sizeof(enemy_ship_pixels) / sizeof(enemy_ship_pixels[0]);
  enemy.move = patterns[rand() % nb_patterns];
  Bullet bullet;
  bullet.size = 3;
  bullet.speed = 6;
  bullet.color = RED;
  bullet.active = 0;

  while (!WindowShouldClose()) {

    deplacer_vaisseau(&ship.position, ship.speed, ship.width, ship.height,
                      screen.width, screen.height);

    shoot_bullet(ship, &bullet);
    update_bullet(&bullet);

    BeginDrawing();
    ClearBackground(BLACK);

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
