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


void _tree_draw_node(Node* base, Node* end);
Vector2 _get_tree_base(void);

void _tree_draw_node(Node* base, Node* child) {
  int lines = base->thickness * TREE_MAX_THICKNESS;

  Vector2 base_dir = {
    .x = cosf(base->rotation + DEG_TO_RAD(90)),
    .y = sinf(base->rotation + DEG_TO_RAD(90))
  };

  Vector2 base_left = VEC_MULT(base_dir, -base->thickness / 2);
  Vector2 base_right = VEC_MULT(base_dir, base->thickness / 2);

  Vector2 child_dir = {
    .x = cosf(child->rotation + DEG_TO_RAD(90)),
    .y = sinf(child->rotation + DEG_TO_RAD(90))
  };

  Vector2 child_left = VEC_MULT(child_dir, -child->thickness / 2);
  Vector2 child_right = VEC_MULT(child_dir, child->thickness / 2);

  base_left = VEC_ADD(base_left, base->position);
  base_right = VEC_ADD(base_right, base->position);
  base_left = VEC_SUB(_get_tree_base(), base_left);
  base_right = VEC_SUB(_get_tree_base(), base_right);

  child_left = VEC_ADD(child_left, child->position);
  child_right = VEC_ADD(child_right, child->position);
  child_left = VEC_SUB(_get_tree_base(), child_left);
  child_right = VEC_SUB(_get_tree_base(), child_right);

  DrawCircleV(_get_tree_base(), 6, RED);
  for (int line = 0; line < lines; line++) {
    Vector2 start = LerpV2(base_left, base_right, (float)line / lines);
    Vector2 end = LerpV2(child_left, child_right, (float)line / lines);

    DrawLineV(start, end, BLUE);
  }
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

void tree_deinit(Tree* tree) {
  if (!tree) return;

  vec_free(&tree->nodes);

  return;
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

int tree_add_node(Tree* tree, size_t id, float rotationOffset, float length) {
  if (!tree) RES_RETURN(r_ENULL_PTR, -1);
  
  Node* base = vec_get(&tree->nodes, id);
  if (base->children_count == TREE_MAX_NODE_CHILDREN) RES_RETURN(r_EBUFFER_FULL, -1);

  float rotation = base->rotation + rotationOffset;

  Vector2 dir = {
    .x = cosf(rotation),
    .y = sinf(rotation)
  };

  Vector2 offset = VEC_MULT(dir, length);

  Node new = {
    .position = VEC_ADD(base->position, offset),
    .thickness = TREE_MAX_THICKNESS,
    .rotation = rotation,
    .children_count = 0,
  };

  base->children[base->children_count++] = tree->nodes.count;
  if (vec_push(&tree->nodes, &new)) return -1;

  RES_RETURN(r_ENONE, 0);
}

