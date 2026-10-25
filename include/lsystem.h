#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <stddef.h>
#include "vector.h"
#include "str.h"

typedef struct {
  char pattern;
  Str replacement;
} LSystemRule;

typedef struct {
  Vec rules;
  Str axiom;
  Str state;
  size_t iteration;
} LSystem;

int lsys_init(LSystem* lsys);
void lsys_free(LSystem* lsys);

int lsys_add_rule(LSystem* lsys, char pattern, Str* replacement);
int lsys_add_cstr_rule(LSystem* lsys, char pattern, char* replacement);
int lsys_remove_rule(LSystem* lsys, size_t ruleIndex);

int lsys_advance(LSystem* lsys, size_t steps);
int lsys_reset(LSystem* lsys);

#endif
