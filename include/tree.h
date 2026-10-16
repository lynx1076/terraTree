#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <raylib.h>
#include "vector.h"

#define TREE_MAX_NODE_CHILDREN        64

typedef struct s_Node Node;

struct s_Node {
  Vector2 position;
  float rotation;
  float thickness;

  size_t children[TREE_MAX_NODE_CHILDREN];
  size_t children_count;
};

typedef struct {
  Vec nodes;
} Tree;


int tree_init(Tree* tree);
int tree_draw(Tree* tree);
int tree_add_node(Tree* tree, size_t id);

#endif
