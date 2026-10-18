#include "lsystem.h"
#include "result.h"
#include <stddef.h>


int lsys_init(LSystem* lsys) {
  if (!lsys) RES_RETURN(r_ENULL_PTR, -1);

  Vec rules;
  if (vec_init(&rules, sizeof(LSystemRule))) return -1;

  Str axiom;
  if (str_init(&axiom)) return -1;
  
  Str state;
  if (str_init(&state)) return -1;

  *lsys = (LSystem){
    .rules = rules,
    .axiom = axiom,
    .state = state,
    .iteration = 0,
  };

  RES_RETURN(r_ENONE, 0);
}

void lsys_free(LSystem* lsys) {
  if (!lsys) return;
  
  for (size_t i = 0; i < lsys->rules.count; i++) {
    LSystemRule* rule = vec_get(&lsys->rules, i);
    str_free(&rule->replacement);
  }
  vec_free(&lsys->rules);

  str_free(&lsys->axiom);
  str_free(&lsys->state);
}

int lsys_add_rule(LSystem* lsys, char pattern, Str* replacement) {
  if (!lsys) RES_RETURN(r_ENULL_PTR, -1);

  LSystemRule lrule;
  lrule.pattern = pattern;

  if (str_dup(&lrule.replacement, replacement)) return -1;

  if (vec_push(&lsys->rules, &lrule)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int lsys_add_cstr_rule(LSystem* lsys, char pattern, char* replacement) {
  if (!lsys) RES_RETURN(r_ENULL_PTR, -1);

  LSystemRule lrule;
  lrule.pattern = pattern;

  if (str_init_cstr(&lrule.replacement, replacement)) return -1;

  if (vec_push(&lsys->rules, &lrule)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int lsys_advance(LSystem* lsys, size_t steps) {
  if (!lsys) RES_RETURN(r_ENULL_PTR, -1);

  for (size_t step = 0; step < steps; step++) {
    size_t skip = 0;
    for (size_t i = 0; i < lsys->state.length; i++) {
      if (skip) {
        skip--;
        continue;
      }

      for (size_t ruleIndex = 0; ruleIndex < lsys->rules.count; ruleIndex++) {
        LSystemRule* rule = vec_get(&lsys->rules, ruleIndex);
        if (lsys->state.text[i] == rule->pattern) {
          if (str_cut(&lsys->state, i, 1)) return -1;
          if (str_insert(&lsys->state, &rule->replacement, i)) return -1;
          if (rule->replacement.length) {
            skip = rule->replacement.length - 1;
            break;
          }
        }
      }
    }
  }

  lsys->iteration += steps;

  RES_RETURN(r_ENONE, 0);
}

int lsys_reset(LSystem* lsys) {
  if (!lsys) RES_RETURN(r_ENULL_PTR, -1);

  lsys->iteration = 0;
  if (str_cpy(&lsys->state, &lsys->axiom)) return -1;

  RES_RETURN(r_ENONE, 0);
}

