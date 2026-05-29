#include <stdint.h>
#include "mt19937.h"

#define MT_N         624
#define MT_M         397
#define MATRIX_A     0x9908b0dfUL
#define UPPER_MASK   0x80000000UL
#define LOWER_MASK   0x7fffffffUL

// L'état interne
static uint32_t mt[MT_N];
static int index = MT_N + 1;

// Fonction 1 — Initialisation avec une graine
void mt_init(uint32_t seed) {
    mt[0] = seed;
    for (int i = 1; i < MT_N; i++) {
        mt[i] = 1812433253UL * (mt[i-1] ^ (mt[i-1] >> 30)) + i;
    }
    index = MT_N;
}

// Fonction 2 — Générer un entier 32 bits
uint32_t mt_generate() {
    // Le twist : recalculer les 624 nombres quand épuisés
    if (index >= MT_N) {
        for (int i = 0; i < MT_N; i++) {
            uint32_t x = (mt[i] & UPPER_MASK) | (mt[(i+1) % MT_N] & LOWER_MASK);
            mt[i] = mt[(i + MT_M) % MT_N] ^ (x >> 1);
            if (x % 2 != 0)
                mt[i] ^= MATRIX_A;
        }
        index = 0;
    }

    // Le temper : améliorer la qualité
    uint32_t y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7)  & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);
    return y;
}

// Fonction 3 — Générer un float entre ]0,1[
float mt_uniform() {
    return (mt_generate() + 0.5f) / 4294967296.0f;  // 2^32
}