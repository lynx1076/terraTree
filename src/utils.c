#include "utils.h"
#include <raylib.h>
#include <stdio.h>

float GetRandomFloat() {
    return (float)GetRandomValue(0, 1000000) / 1000000.0f;
}

float GetRandomFloatRange(float min, float max) {
    return min + ((float)GetRandomValue(0, 1000000) / 1000000.0f) * (max - min);
}

void PrintVector(Vector2 v) {
  printf("%f, %f", v.x, v.y);
}

