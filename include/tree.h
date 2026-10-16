#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <raylib.h>
#include "vector.h"

#define TREE_VERTEX_MAX_CHILDREN      8

typedef struct s_Vertex Vertex;

struct s_Vertex {
  Vector2 position;
  float rotation;
  float thickness;
  size_t children[TREE_VERTEX_MAX_CHILDREN];
};

typedef struct {
  Vec vertices;
} Tree;


int tree_init(Tree* tree);
int tree_draw(Tree* tree);
int tree_add_branch(Tree* tree, size_t id);
int tree_print(Tree* tree);

#endif
