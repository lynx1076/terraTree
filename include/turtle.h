#ifndef TURTLE_H
#define TURTLE_H

#include <stddef.h>
#include <raylib.h>
#include <stdbool.h>
#include "vector.h"

typedef enum {
  TInT_None,
  TInT_Move,
  TInT_MoveNoDraw,
  TInT_Rotate,
  TInT_Push,
  TInT_Pop
} TurtleInstructionType;

typedef struct {
  TurtleInstructionType type;
  union {
    float length;
    float angle;
  };
} TurtleInstruction;

typedef struct {
  Vector2 position;
  float rotation;
} TurtleTransform;

typedef struct {
  Color color;
  float thickness;

  Vec instructions;
  Vec transforms;

  TurtleTransform startTransform;
  TurtleTransform transform;

  bool showTurtle;
  size_t currentStep;
} Turtle;

int turtle_init(Turtle* turtle, TurtleTransform startTransform);
void turtle_free(Turtle* turtle);

int turtle_reset(Turtle* turtle);

int turtle_move(Turtle* turtle, float length, bool penDown);
int turtle_rotate(Turtle* turtle, float rotation);
int turtle_push(Turtle* turtle);
int turtle_pop(Turtle* turtle);

int turtle_show(Turtle* turtle, bool show);

int turtle_draw(Turtle* turtle);
int turtle_draw_step(Turtle* turtle);
int turtle_drawing_reset(Turtle* turtle);
bool turtle_is_drawing(Turtle* turtle);

#endif
