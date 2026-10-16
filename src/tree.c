#include "tree.h"
#include "result.h"
#include "utils.h"
#include "main.h"
#include <math.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>


#define TREE_MAX_ANGLE        DEG_TO_RAD(45)


void _tree_draw_branch(Vertex* v_base, Vertex* v_branch);


void _tree_draw_branch(Vertex* v_base, Vertex* v_branch) {
  Vector2 windowPosStart = (Vector2){
    (0.5f + v_base->position.x) * window_width,
    (1 - v_base->position.y) * window_height
  };

  Vector2 windowPosEnd = (Vector2){
    (0.5f + v_branch->position.x) * window_width,
    (1 - v_branch->position.y) * window_height
  };

  DrawLineV(windowPosStart, windowPosEnd, BROWN);
}

int tree_init(Tree* tree) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);
  if (vec_init(&tree->vertices, sizeof(Vertex))) return -1;

  Vertex base = {
    .position = (Vector2){0, 0},
    .thickness = 10,
    .rotation = 0
  };

  for (int i = 0; i < TREE_VERTEX_MAX_CHILDREN; i++) {
    base.children[i] = 0;
  }

  if (vec_push(&tree->vertices, &base)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int tree_draw(Tree* tree) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);

  size_t vcount = tree->vertices.count;

  for (size_t i = 0; i < vcount - 1; i++) {
    Vertex* v_base = vec_get(&tree->vertices, i);
    size_t j = 0;
    while (v_base->children[j] != 0) {
      Vertex* v_branch = vec_get(&tree->vertices, v_base->children[j]);
      _tree_draw_branch(v_base, v_branch);
      j++;
    }
  }

  RES_RETURN(r_ENONE, 0);
}

int tree_add_branch(Tree* tree, size_t id) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);
  
  Vertex* v_base = vec_get(&tree->vertices, id);

  int child_index = 0;
  for (; v_base->children[child_index] != 0; child_index++) {
    if (child_index > TREE_VERTEX_MAX_CHILDREN) RES_RETURN(r_EBUFFER_FULL, -1);
  }

  Vertex v_new;

  v_new.rotation = GetRandomFloatRange(-TREE_MAX_ANGLE, TREE_MAX_ANGLE);
  v_new.thickness = GetRandomFloatRange(0.1f, 0.6f);

  float length = GetRandomFloatRange(0.03f, 0.3f);

  v_new.position.x = cosf(v_new.rotation + DEG_TO_RAD(90)) * length + v_base->position.x;
  v_new.position.y = sinf(v_new.rotation + DEG_TO_RAD(90)) * length + v_base->position.y;

  for (int i = 0; i < TREE_VERTEX_MAX_CHILDREN; i++) {
    v_new.children[i] = 0;
  }

  if (vec_push(&tree->vertices, &v_new)) return -1;

  v_base->children[child_index] = tree->vertices.count - 1;

  RES_RETURN(r_ENONE, 0);
}

int tree_print(Tree* tree) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);

  for (size_t i = 0; i < tree->vertices.count; i++) {
    Vertex* v = vec_get(&tree->vertices, i);
    
    printf("Vertex: %zu @ %f, %f\n", i, v->position.x, v->position.y);

    for (int j = 0; v->children[j] != 0; j++) {
      Vertex* child = vec_get(&tree->vertices, v->children[j]);
      printf("\t->id: %zu @ %f, %f\n", v->children[j], child->position.x, child->position.y);
    }
    printf("\n");
  }

  RES_RETURN(r_ENONE, 0);
}

