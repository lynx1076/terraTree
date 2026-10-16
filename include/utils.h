#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>


#define DEG_TO_RAD(DEG)         (DEG * PI / 180)
#define RAD_TO_DEG(RAD)         (RAD * 180 / PI)

#define VEC_ADD(A, B)           ((Vector2){(A).x + (B).x, (A).y + (B).y})
#define VEC_SUB(A, B)           ((Vector2){(A).x - (B).x, (A).y - (B).y})
#define VEC_MULT(A, X)          ((Vector2){(A).x * (X), (A).y * (X)})


float GetRandomFloat();
float GetRandomFloatRange(float min, float max);
void PrintVector(Vector2 v);

#endif
