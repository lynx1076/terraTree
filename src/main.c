#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include "utils.h"
#include "turtle.h"
#include "result.h"
#include "str.h"
#include "lsystem.h"

#define ANGLE DEG_TO_RAD(89.11022009)

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

  if (lsys_add_cstr_rule(&lsys, 'X', "X+YF+")) return _res;
  if (lsys_add_cstr_rule(&lsys, 'Y', "-FX-Y")) return _res;

  if (str_ccpy(&lsys.axiom, "FX")) return _res;
  if (lsys_reset(&lsys)) return _res;

  if (lsys_advance(&lsys, 19)) return -1;
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

  if (turtle_init(&turtle, (TurtleTransform){{0, -200}, DEG_TO_RAD(-90)})) return _res;
  if (turtle_show(&turtle, false)) return _res;

  turtle.color = GREEN;

  for (size_t i = 0; i < lsys.state.length; i++) {
    switch (lsys.state.text[i]) {
      case 'F': {
        if (turtle_move(&turtle, 10, true)) return _res;
      } break;
      case '[': {
        if (turtle_push(&turtle)) return _res;
      } break;
      case ']': {
        if (turtle_pop(&turtle)) return _res;
      } break;
      case '+': {
        if (turtle_rotate(&turtle, ANGLE)) return _res;
      } break;
      case '-': {
        if (turtle_rotate(&turtle, -ANGLE)) return _res;
      } break;
      default: break;
    }
  }

  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    window_width = GetScreenWidth();
    window_height = GetScreenHeight();
    camera.offset = (Vector2){window_width / 2.f, window_height / 1.f};

    camera.zoom *= (1 - GetMouseWheelMove() / 2);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))  {
      Vector2 move = VEC_MULT(GetMouseDelta(), 1 / camera.zoom);
      camera.target = VEC_SUB(camera.target, move);
    }
    
    BeginDrawing();
    ClearBackground(WHITE);

    BeginMode2D(camera);
    if (turtle_draw(&turtle)) return _res;
    EndMode2D();

    EndDrawing();
  }

  turtle_free(&turtle);
  lsys_free(&lsys);

  return 0;
}

