#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>


#define DEG_TO_RAD(DEG)        (DEG * PI / 180)
#define RAD_TO_DEG(RAD)        (RAD * 180 / PI)



float GetRandomFloat();
float GetRandomFloatRange(float min, float max);

#endif
