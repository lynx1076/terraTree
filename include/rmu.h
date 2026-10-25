#ifndef RMU_H
#define RMU_H

#include <raylib.h>
#include <microui.h>

#define FONT_SIZE       12
#define SCROLL_SPEED    20

int rmu_text_width(mu_Font font, const char *text, int len);
int rmu_text_height(mu_Font font);
void rmu_input(mu_Context *ctx);
void rmu_render(mu_Context *ctx);

#endif
