#include "gui.h"
#include "lsystem.h"
#include "result.h"
#include "microui.h"
#include "rmu.h"
#include "main.h"
#include "str.h"
#include "turtle.h"
#include "vector.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#define UI_WIDTH            300
#define INPUT_BUFF_SIZE     256

const char* format_bytes(size_t bytes);

mu_Context ctx;

char new_rule_pattern[2];
char new_rule_replacement[INPUT_BUFF_SIZE];
char new_axiom[INPUT_BUFF_SIZE];

char forward[2] = "F";
char left[2] = "-";
char right[2] = "+";
char push[2] = "[";
char pop[2] = "]";


int gui_init(void) {
  mu_init(&ctx);

  ctx.text_width = rmu_text_width;
  ctx.text_height = rmu_text_height;

  RES_RETURN(r_ENONE, 0);
}

const char* format_bytes(size_t bytes) {
  const char *suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB"};
  int i = 0;
  double count = bytes;

  while (count >= 1024 && i < 5) {
    i++;
    count /= 1024;
  }

  if (i == 0) {
    return TextFormat("%zu %s\n", bytes, suffixes[i]);
  } else {
    return TextFormat("%.2f %s\n", count, suffixes[i]);
  }
}

int gui_update(LSystem* lsys, Turtle* turtle) {
  rmu_input(&ctx);
  mu_begin(&ctx);

  
  if (mu_begin_window_ex(&ctx, "", mu_rect(window_width - UI_WIDTH, 0, UI_WIDTH, window_height), MU_OPT_NOCLOSE | MU_OPT_NOTITLE | MU_OPT_NORESIZE)) {
    mu_Container *cnt = mu_get_current_container(&ctx);
    cnt->rect.x = window_width - UI_WIDTH;
    cnt->rect.h = window_height;
    
    mu_layout_row(&ctx, 1, (int[]) { -1 }, 30);
    mu_label(&ctx, "L-System");

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 400);
    mu_begin_panel(&ctx, "Rules");
    for (size_t i = 0; i < lsys->rules.count; i++) {
      LSystemRule* rule = vec_get(&lsys->rules, i);
      char pattern_str[2] = {0, 0};
      pattern_str[0] = rule->pattern;

      mu_layout_row(&ctx, 4, (int[]) { FONT_SIZE * 10, FONT_SIZE, FONT_SIZE * 4, -1 }, 0);
      bool delete_curr = mu_button(&ctx, TextFormat("Delete %zu", i + 1));
      mu_label(&ctx, pattern_str);
      mu_label(&ctx, " -> ");
      if (str_cstr(&rule->replacement)) return -1;
      mu_label(&ctx, rule->replacement.text);

      if (delete_curr) {
        printf("Removing rule: %zu\n", i);
        if (lsys_remove_rule(lsys, i)) return -1;
      }
    }
    mu_end_panel(&ctx);

    mu_layout_row(&ctx, 4, (int[]) { FONT_SIZE * 6, FONT_SIZE * 2, FONT_SIZE * 8, -1 }, 0);
    mu_label(&ctx, "Pattern: ");
    mu_textbox(&ctx, new_rule_pattern, 2);

    mu_label(&ctx, "Replacement: ");
    mu_textbox(&ctx, new_rule_replacement, INPUT_BUFF_SIZE);

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 0);
    if (mu_button(&ctx, "Add rule")) {
      if (!strlen(new_rule_pattern)) printf("Cannot add rule with empty pattern\n");
      else {
        if (lsys_add_cstr_rule(lsys, *new_rule_pattern, new_rule_replacement)) return -1;
      }
    }

    mu_layout_row(&ctx, 2, (int[]) { FONT_SIZE * 10, -1 }, 0);
    mu_label(&ctx, "Current axiom: ");
    str_cstr(&lsys->axiom);
    mu_label(&ctx, lsys->axiom.text);

    if (mu_button(&ctx, "Set axiom")) {
      if (!strlen(new_axiom)) printf("Cannot set empty axiom\n");
      else if (str_ccpy(&lsys->axiom, new_axiom)) return -1;
    }
    mu_textbox(&ctx, new_axiom, INPUT_BUFF_SIZE);

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 0);
    if (mu_button(&ctx, "Reset L-System")) {
      if (lsys_reset(lsys)) return -1;
    }

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 0);
    if (lsys->iteration > 15) {
      mu_label(&ctx, "Warning: lag may occurr");
    }

    if (mu_button(&ctx, "Step L-System")) {
      if (lsys_advance(lsys, 1)) return -1;
    }

    bool draw = false;
    if (mu_button(&ctx, "Step and draw")) {
      if (lsys_advance(lsys, 1)) return -1;
      draw = true;
    }

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 30);
    mu_label(&ctx, "Turtle");

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 0);
    mu_label(&ctx, TextFormat("Angle: %.2f°", turtle->angle));
    mu_layout_row(&ctx, 2, (int[]) { FONT_SIZE * 8, FONT_SIZE * 8 }, 0);
    if (mu_button(&ctx, "Angle++")) turtle->angle++;
    if (mu_button(&ctx, "Angle--")) turtle->angle--;

    mu_layout_row(&ctx, 2, (int[]) { FONT_SIZE * 8, FONT_SIZE * 8 }, 0);
    if (mu_button(&ctx, "Angle+")) turtle->angle += 0.1f;
    if (mu_button(&ctx, "Angle-")) turtle->angle += 0.1f;

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 0);
    if (mu_button(&ctx, "Redraw") || draw) {
      if (turtle_update(turtle, lsys)) return -1;
    }

    mu_layout_row(&ctx, 2, (int[]) { FONT_SIZE * 10, -1 }, 0);
    mu_label(&ctx, "Foward");
    mu_textbox(&ctx, forward, 2);
    turtle->forward = *forward;

    mu_label(&ctx, "Left");
    mu_textbox(&ctx, left, 2);
    turtle->left = *left;

    mu_label(&ctx, "Right");
    mu_textbox(&ctx, right, 2);
    turtle->right = *right;

    mu_layout_row(&ctx, 2, (int[]) { FONT_SIZE * 10, -1 }, 0);
    mu_label(&ctx, "Push");
    mu_textbox(&ctx, push, 2);
    turtle->push = *push;

    mu_label(&ctx, "Pop");
    mu_textbox(&ctx, pop, 2);
    turtle->pop = *pop;

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 30);
    mu_label(&ctx, "Stats");

    mu_layout_row(&ctx, 1, (int[]) { -1 }, 0);
    mu_label(&ctx, TextFormat("L-System iteration: %zu", lsys->iteration));
    mu_label(&ctx, TextFormat("L-System length: %zu", lsys->state.length));
    mu_layout_row(&ctx, 2, (int[]) { FONT_SIZE * 14, -1 }, 0);
    mu_label(&ctx, "L-System memory usage: ");
    mu_label(&ctx, format_bytes(lsys->state.length));

    mu_end_window(&ctx);
  }

  mu_end(&ctx);
  rmu_render(&ctx);


  RES_RETURN(r_ENONE, 0);
}

