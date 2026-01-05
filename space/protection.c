#include "protection.h"
#include "config.h"
#include "draw.h"
#include "raylib.h"

void init_protection(Protection protection[]) {
  float spacingX = SCREEN_WIDTH / (float)(MAX_PROTECTION + 1);
  float baseY = SCREEN_HEIGHT * 0.75f;

  for (int i = 0; i < MAX_PROTECTION; i++) {
    protection[i].width = 21;
    protection[i].height = 9;
    protection[i].life = PROTECTION_LIFE;

    protection[i].position =
        (Vector2){spacingX * (i + 1) - protection[i].width / 2.0f, baseY};
  }
}

void draw_protection(Protection protection[]) {
  for (int i = 0; i < MAX_PROTECTION; i++) {
    if (protection[i].life == 0)
      continue;

    for (int j = 0; j < PROTECTION_LIFE; j++) {
      draw_pixels(protection[i].position, 6, BLUE, 3, 7, protection_shape[j]);
      if (protection[i].life - 1 == j)
        break;
    }
  }
}

void update_protection(Protection protection[]) {
  // Currently no dynamic behavior for protections
}
