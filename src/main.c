#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include "utils.h"
#include "turtle.h"
#include "result.h"
#include "str.h"
#include "lsystem.h"


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

  if (lsys_add_cstr_rule(&lsys, 'X', "F+[[X]-X]-F[-FX]+X")) return _res;
  if (lsys_add_cstr_rule(&lsys, 'F', "FF")) return _res;

  if (str_ccpy(&lsys.axiom, "X")) return _res;
  if (lsys_reset(&lsys)) return _res;

  for (int i = 0; i < 6; i++) {
    if (str_cstr(&lsys.state)) return _res;
    printf("State [%zu] (%zu): %s\n", lsys.iteration, lsys.state.length, lsys.state.text);

    if (lsys_advance(&lsys, 1)) return -1;
  }

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

  if (turtle_init(&turtle, (TurtleTransform){{0, 0}, DEG_TO_RAD(-90)})) return _res;
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
        if (turtle_rotate(&turtle, DEG_TO_RAD(25))) return _res;
      } break;
      case '-': {
        if (turtle_rotate(&turtle, -DEG_TO_RAD(25))) return _res;
      } break;

      case 'X':
      default: break;
    }
  }

  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    window_width = GetScreenWidth();
    window_height = GetScreenHeight();
    camera.offset = (Vector2){window_width / 2.f, window_height / 1.f};
    
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

