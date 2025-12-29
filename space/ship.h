#ifndef SHIP_H
#define SHIP_H

#include "bullet.h"
#include "raylib.h"

typedef struct {
  Vector2 position;
  float speed;
  int width, height;
  int player_lifes;
} Player;

void deplacer_vaisseau(Vector2 *pos, float speed, int ship_width,
                       int ship_height, int screenWidth, int screenHeight);
void player_init(Player *player);
void move_ship(Vector2 *pos, float speed, int ship_width, int ship_height,
               int screenWidth, int screenHeight);
void shoot_bullet(Player player, Bullet bullets[]);

#endif
