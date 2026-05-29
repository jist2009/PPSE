#ifndef MT19937_H
#define MT19937_H

#include <stdint.h>

void     mt_init(uint32_t seed);
uint32_t mt_generate();
float    mt_uniform();

#endif