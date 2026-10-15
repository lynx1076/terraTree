#include <raylib.h>
#include <stddef.h>
#include "vector.h"
#include "result.h"

typedef struct s_Vertex Vertex;

struct s_Vertex {
  size_t id;
  Vector2 position;
  float normal_direction;
  float thickness;
  size_t children;
};

int window_width = 800;
int window_height = 600;

char* window_title = "Test";

Vec tree = VEC_EMPTY;

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  if (vec_init(&tree, sizeof(Vertex))) return _res;

  Vertex base = {
    .id = 0,
    .position = (Vector2){0, 0},
    .normal_direction = 0,
    .children = 0
  };
  if (vec_push(&tree, &base)) return _res;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(window_width, window_height, window_title);
  SetExitKey(KEY_NULL);

  if (!IsWindowReady()) {
    return 0;
  }

  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    BeginDrawing();

    EndDrawing();
  }

  return 0;
}

