#include <stdint.h>
#include <math.h>
#include <arm_neon.h>
#include "mt19937.h"
#include "neon_mathfun.h"
#include "project.h"

void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma) {

    // constantes NEON
    float32x4_t v_sigma    = vdupq_n_f32(sigma);
    float32x4_t v_moins2   = vdupq_n_f32(-2.0f);
    float32x4_t v_2pi      = vdupq_n_f32(2.0f * M_PI);

    // 8 éléments par tour (Z0 pour 4 + Z1 pour 4)
    for (size_t i = 0; i <= N; i += 8) {

        // générer U1[4] et U2[4] avec MT
        float u1_arr[4], u2_arr[4];
        for (int k = 0; k < 4; k++) {
            u1_arr[k] = mt_uniform();
            u2_arr[k] = mt_uniform();
        }

        // charger U1 et U2 dans des registres NEON
        float32x4_t U1 = vld1q_f32(u1_arr);
        float32x4_t U2 = vld1q_f32(u2_arr);

        // calculer r = sqrt(-2 * log(U1)) 
        // log_ps : logarithme librairie Julien Pommier
        float32x4_t log_U1 = log_ps(U1);
        // multiplier par -2
        float32x4_t val    = vmulq_f32(v_moins2, log_U1);
        // sqrt 
        float32x4_t r      = vsqrtq_f32(val);

        // calculer cos et sin 
        // 2*pi*U2
        float32x4_t angle  = vmulq_f32(v_2pi, U2);
        // calcule sin ET cos en même temps
        float32x4_t vcos, vsin;
        sincos_ps(angle, &vsin, &vcos);

        // Z0 = r * cos,  Z1 = r * sin 
        float32x4_t Z0 = vmulq_f32(r, vcos);
        float32x4_t Z1 = vmulq_f32(r, vsin);

        // charger X_N et ajouter le bruit 
        // X_N est int32 on le convertit en float
        float32x4_t x0 = vcvtq_f32_s32(vld1q_s32(X_N + i));
        float32x4_t x1 = vcvtq_f32_s32(vld1q_s32(X_N + i + 4));

        // Y_N = X_N + sigma * Z
        float32x4_t y0 = vaddq_f32(x0, vmulq_f32(v_sigma, Z0));
        float32x4_t y1 = vaddq_f32(x1, vmulq_f32(v_sigma, Z1));

        // stocker les résultats
        vst1q_f32(Y_N + i,     y0);
        vst1q_f32(Y_N + i + 4, y1);
    }

}