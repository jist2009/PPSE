#ifndef NEON_MATHFUN_H
#define NEON_MATHFUN_H

#include <arm_neon.h>
typedef float32x4_t v4sf;
typedef uint32x4_t v4su;
typedef int32x4_t v4si;

// logarithme sur 4 floats
v4sf log_ps(v4sf x);

// exponentielle sur 4 floats
v4sf exp_ps(v4sf x);

// sin et cos en même temps sur 4 floats
void sincos_ps(v4sf x, v4sf *ysin, v4sf *ycos);

// sin sur 4 floats
v4sf sin_ps(v4sf x);

// cos sur 4 floats
v4sf cos_ps(v4sf x);

#endif