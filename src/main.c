#include <raylib.h>
#include "tree.h"
#include "result.h"


int window_width = 800;
int window_height = 600;

char* window_title = "Test";

Tree tree;

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(window_width, window_height, window_title);
  SetExitKey(KEY_NULL);

  if (tree_init(&tree)) return _res;
  if (tree_add_branch(&tree, 0)) return _res;
  if (tree_add_branch(&tree, 1)) return _res;
  if (tree_add_branch(&tree, 2)) return _res;
  if (tree_add_branch(&tree, 2)) return _res;
  if (tree_add_branch(&tree, 1)) return _res;

  tree_print(&tree);

  if (!IsWindowReady()) {
    return 0;
  }

  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    window_width = GetScreenWidth();
    window_height = GetScreenHeight();

    BeginDrawing();
    ClearBackground(WHITE);

    tree_draw(&tree);

    EndDrawing();
  }

  return 0;
}

