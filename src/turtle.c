#include "turtle.h"
#include "lsystem.h"
#include "result.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <stddef.h>

int _turtle_add_instruction(Turtle* turtle, TurtleInstruction* instruction);


int _turtle_add_instruction(Turtle* turtle, TurtleInstruction* instruction) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  if (vec_push(&turtle->instructions, instruction)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int turtle_init(Turtle* turtle, TurtleTransform startTransform) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  Vec instructions = VEC_EMPTY;
  if (vec_init(&instructions, sizeof(TurtleInstruction))) return -1;

  Vec transforms = VEC_EMPTY;
  if (vec_init(&transforms, sizeof(TurtleTransform))) return -1;

  turtle->currentStep = 0;

  *turtle = (Turtle){
    .instructions = instructions,
    .transforms = transforms,
    .color = RED,
    .thickness = 10,
    .startTransform = startTransform,
    .transform = startTransform,
  };

  RES_RETURN(r_ENONE, 0);
}

void turtle_free(Turtle* turtle) {
  if (!turtle) return;

  vec_free(&turtle->instructions);
}

int turtle_reset(Turtle* turtle) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  if (vec_clear(&turtle->instructions)) return -1;
  if (vec_clear(&turtle->transforms)) return -1;

  turtle->transform = turtle->startTransform;

  turtle->currentStep = 0;

  RES_RETURN(r_ENONE, 0);
}

int turtle_move(Turtle* turtle, float length, bool penDown) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  TurtleInstruction instruction = {
    .type = penDown ? TInT_Move : TInT_MoveNoDraw,
    .length = length,
  };

  if (_turtle_add_instruction(turtle, &instruction)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int turtle_rotate(Turtle* turtle, float rotation) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  TurtleInstruction instruction = {
    .type = TInT_Rotate,
    .angle = rotation,
  };

  if (_turtle_add_instruction(turtle, &instruction)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int turtle_push(Turtle* turtle) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  TurtleInstruction instruction = {
    .type = TInT_Push,
  };

  if (_turtle_add_instruction(turtle, &instruction)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int turtle_pop(Turtle* turtle) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  TurtleInstruction instruction = {
    .type = TInT_Pop,
  };

  if (_turtle_add_instruction(turtle, &instruction)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int turtle_show(Turtle* turtle, bool show) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);
  turtle->showTurtle = show;

  RES_RETURN(r_ENONE, 0);
}

int turtle_draw(Turtle* turtle) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);
  if (turtle_drawing_reset(turtle)) return -1;

  for (size_t i = 0; i < turtle->instructions.count; i++) {
    if (turtle_draw_step(turtle)) return -1;
  }
  if (turtle->showTurtle) DrawCircleV(turtle->transform.position, 5, BLUE);

  RES_RETURN(r_ENONE, 0);
}

int turtle_draw_step(Turtle* turtle) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);
  if (turtle->currentStep >= turtle->instructions.count) RES_RETURN(r_EBOUNDS, 0);

  TurtleInstruction* instruction = vec_get(&turtle->instructions, turtle->currentStep);

  switch (instruction->type) {
    case TInT_MoveNoDraw:
    case TInT_Move: {
      Vector2 dir = {
        cosf(DEG_TO_RAD(turtle->transform.rotation)),
        sinf(DEG_TO_RAD(turtle->transform.rotation))
      };
      Vector2 delta = VEC_MULT(dir, instruction->length);
      Vector2 newPos = VEC_ADD(turtle->transform.position, delta);

      if (instruction->type != TInT_MoveNoDraw) DrawLineV(turtle->transform.position, newPos, turtle->color);
      turtle->transform.position = newPos;
    } break;
    case TInT_Rotate: {
      turtle->transform.rotation += instruction->angle;
    } break;
    case TInT_Push: {
      if (vec_push(&turtle->transforms, &turtle->transform)) return -1;
    } break;
    case TInT_Pop: {
      if (turtle->transforms.count == 0) break;
      if (vec_pop(&turtle->transforms, &turtle->transform)) return -1;
    } break;
    case TInT_None:
    default: break;
  }

  turtle->currentStep++;

  RES_RETURN(r_ENONE, 0);
}

int turtle_drawing_reset(Turtle* turtle) {
  if (!turtle) RES_RETURN(r_ENULL_PTR, -1);

  turtle->transform = turtle->startTransform;
  turtle->currentStep = 0;

  RES_RETURN(r_ENONE, 0);
}

bool turtle_is_drawing(Turtle* turtle) {
  if (!turtle) return false;

  return turtle->currentStep >= turtle->instructions.count;
}

int turtle_update(Turtle* turtle, const LSystem* lsys) {
  if (turtle_reset(turtle)) return -1;
  for (size_t i = 0; i < lsys->state.length; i++) {
    if (lsys->state.text[i] == turtle->forward) {
      if (turtle_move(turtle, turtle->stepsize, true)) return -1;
    } 
    if (lsys->state.text[i] == turtle->push) {
      if (turtle_push(turtle)) return -1;
    } 
    if (lsys->state.text[i] == turtle->pop) {
      if (turtle_pop(turtle)) return -1;
    } 
    if (lsys->state.text[i] == turtle->right) {
      if (turtle_rotate(turtle, turtle->angle)) return -1;
    } 
    if (lsys->state.text[i] == turtle->left) {
      if (turtle_rotate(turtle, -turtle->angle)) return -1;
    }
  }

  RES_RETURN(r_ENONE, 0);
}

