#include <raylib.h>
#include "tree.h"
#include "result.h"


int window_width = 800;
int window_height = 600;

char* window_title = "terraTree";

Tree tree;

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  SetTraceLogLevel(LOG_ERROR);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(window_width, window_height, window_title);
  SetExitKey(KEY_NULL);

  if (!IsWindowReady()) {
    return 0;
  }

  if (tree_init(&tree)) return _res;

  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    window_width = GetScreenWidth();
    window_height = GetScreenHeight();

    BeginDrawing();
    ClearBackground(GRAY);

    tree_draw(&tree);

    EndDrawing();
  }

  tree_deinit(&tree);

  return 0;
}

