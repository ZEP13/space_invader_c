#include "protection.h"
#include "config.h"
#include "draw.h"
#include "raylib.h"

void init_protection(Protection protection[]) {
  float spacingX = SCREEN_WIDTH / (float)(MAX_PROTECTION + 1);
  float baseY = SCREEN_HEIGHT * 0.75f;

  for (int i = 0; i < MAX_PROTECTION; i++) {
    protection[i].width = PROTECTION_COL * PROTECTION_SCALE;
    protection[i].height = PROTECTION_ROW * PROTECTION_SCALE;
    protection[i].life = PROTECTION_LIFE;

    protection[i].position =
        (Vector2){spacingX * (i + 1) - protection[i].width / 2.0f, baseY};
  }
}

void draw_protection(Protection protection[]) {
  for (int i = 0; i < MAX_PROTECTION; i++) {
    if (protection[i].life == 0)
      continue;

    int index = PROTECTION_LIFE - protection[i].life;
    draw_pixels(protection[i].position, PROTECTION_SCALE, BLUE, PROTECTION_ROW,
                PROTECTION_ROW, protection_shape[index]);
  }
}
