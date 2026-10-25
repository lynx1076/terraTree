#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include "gui.h"
#include "utils.h"
#include "turtle.h"
#include "result.h"
#include "str.h"
#include "lsystem.h"

#define ANGLE DEG_TO_RAD(90)

int window_width = 800;
int window_height = 600;

char* window_title = "terraTree";

Camera2D camera;
Turtle turtle;

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  LSystem lsys;
  if (lsys_init(&lsys)) return _res;

  if (lsys_add_cstr_rule(&lsys, 'X', "X+YF+")) return _res; // Rule 1: X => X+YF+
  if (lsys_add_cstr_rule(&lsys, 'Y', "-FX-Y")) return _res; // Rule 2: Y => -FX-Y

  if (str_ccpy(&lsys.axiom, "FX")) return _res; // Axiom
  if (lsys_reset(&lsys)) return _res;

  if (lsys_advance(&lsys, 10)) return -1;
  printf("Moves: %zu\n", lsys.state.length);

  SetTraceLogLevel(LOG_ERROR);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(window_width, window_height, window_title);
  SetExitKey(KEY_NULL);

  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){window_width / 2.f, window_height};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  if (!IsWindowReady()) {
    return 0;
  }

  if (turtle_init(&turtle, (TurtleTransform){{0, -200}, -90})) return _res;
  if (turtle_show(&turtle, false)) return _res;

  turtle.color = GREEN;
  turtle.stepsize = 10;
  turtle.angle = 90;

  turtle.forward = 'F';
  turtle.left = '-';
  turtle.right = '+';
  turtle.push = '[';
  turtle.pop = ']';

  if (turtle_update(&turtle, &lsys)) return _res;

  SetTargetFPS(30);
  if (gui_init()) return _res;

  while (!WindowShouldClose()) {
    window_width = GetScreenWidth();
    window_height = GetScreenHeight();
    camera.offset = (Vector2){window_width / 2.f, window_height / 1.f};

    camera.zoom *= (1 - GetMouseWheelMove() / 2);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))  {
      Vector2 move = VEC_MULT(GetMouseDelta(), 1 / camera.zoom);
      camera.target = VEC_SUB(camera.target, move);
    }

    if (IsKeyPressed(KEY_SPACE)) {
      camera.zoom = 1.0;
      camera.target = (Vector2){0, 0};
    }
    
    BeginDrawing();
    ClearBackground(WHITE);

    BeginMode2D(camera);
    if (turtle_draw(&turtle)) return _res;
    EndMode2D();

    gui_update(&lsys, &turtle);

    EndDrawing();
  }

  turtle_free(&turtle);
  lsys_free(&lsys);

  return 0;
}

