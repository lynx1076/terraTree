#include "tree.h"
#include "result.h"
#include "utils.h"
#include "main.h"
#include <math.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>


#define TREE_MAX_ANGLE                45
#define TREE_MAX_THICKNESS            20
#define TREE_NODE_OFFSET_MULT         12
#define TREE_MAX_PIXEL_THICKNESS      10


void _tree_draw_node(Node* base, Node* end);
Vector2 _get_tree_base(void);

void _tree_draw_node(Node* base, Node* child) {
  int lines = base->thickness * TREE_MAX_PIXEL_THICKNESS;

  Vector2 dir = {
    .x = cosf(base->rotation),
    .y = sinf(base->rotation)
  };

  Vector2 left = VEC_MULT(dir, -base->thickness / 2);
  Vector2 right = VEC_MULT(dir, base->thickness / 2);

  left = VEC_SUB(_get_tree_base(), left);
  right = VEC_SUB(_get_tree_base(), right);

  DrawCircleV(_get_tree_base(), 6, RED);
  DrawLineV(left, right, GREEN);

  Vector2 start = VEC_SUB(_get_tree_base(), base->position);
  Vector2 end = VEC_SUB(_get_tree_base(), child->position);

  DrawLineV(start, end, BLUE);
}

Vector2 _get_tree_base(void) {
  return (Vector2){window_width / 2.f, window_height};
}

int tree_init(Tree* tree) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);
  if (vec_init(&tree->nodes, sizeof(Node))) return -1;

  Node base = {
    .position = (Vector2){0, 0},
    .thickness = TREE_MAX_THICKNESS,
    .rotation = DEG_TO_RAD(90),
    .children_count = 0,
  };

  if (vec_push(&tree->nodes, &base)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int tree_draw(Tree* tree) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);

  size_t bcount = tree->nodes.count;

  for (size_t i = 0; i < bcount; i++) {
    Node* b = vec_get(&tree->nodes, i);
    for (size_t j = 0; j < b->children_count; j++) {
      _tree_draw_node(b, vec_get(&tree->nodes, b->children[j]));
    }
  }

  RES_RETURN(r_ENONE, 0);
}

int tree_add_node(Tree* tree, size_t id) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);
  
  Node* base = vec_get(&tree->nodes, id);
  if (base->children_count == TREE_MAX_NODE_CHILDREN) RES_RETURN(r_EBUFFER_FULL, -1);

  float length = base->thickness * TREE_NODE_OFFSET_MULT * GetRandomFloatRange(0.3f, 0.9f);

  float rotationOffset = GetRandomFloatRange(-DEG_TO_RAD(TREE_MAX_ANGLE), DEG_TO_RAD(TREE_MAX_ANGLE));
  float rotation = base->rotation + rotationOffset;
  printf("New node with len %f and rot %f rel rot %f\n", length, RAD_TO_DEG(rotation), RAD_TO_DEG(rotationOffset));

  Vector2 dir = {
    .x = cosf(rotation),
    .y = sinf(rotation)
  };

  Vector2 offset = VEC_MULT(dir, length);

  Node new = {
    .position = VEC_ADD(base->position, offset),
    .thickness = base->thickness * GetRandomFloatRange(0.6f, 0.9f),
    .rotation = rotation,
    .children_count = 0,
  };

  base->children[base->children_count++] = tree->nodes.count;
  if (vec_push(&tree->nodes, &new)) return -1;

  RES_RETURN(r_ENONE, 0);
}

