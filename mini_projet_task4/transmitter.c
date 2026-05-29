#include <arm_neon.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "project.h"

// write into the buffer U_K
void source_generate(uint8_t *U_K, size_t K){
   for (size_t i = 0; i < K/8; i++) {
        U_K[i] = rand() % 256;  // 8 bits aléatoires dans 1 octet
    }
}
/*
void source_generate(uint8_t *U_K, size_t K){
    for (size_t i = 0; i < K; i++) {
        U_K[i] = rand() % 2 ; // Génère un nombre aléatoire : 0 ou 1
    }
}
*/

// read from the buffer U_K and write into the buffer C_N
void codec_repetition_encode(const uint8_t *U_K, uint8_t *C_N, size_t K, size_t n_reps){
   for (size_t i = 0; i < n_reps; i++) {
       for (size_t j =0; j < K/8; j++) {
           C_N[i*K/8 + j] = U_K[j]; // Répète U_K n_reps fois dans C_N
        }
    }
}
/*
void codec_repetition_encode(const uint8_t *U_K, uint8_t *C_N, size_t K, size_t n_reps){
    for (size_t i = 0; i < n_reps; i++) {
        for (size_t j =0; j < K; j++) {
            C_N[i*K + j] = U_K[j]; // Répète U_K n_reps fois dans C_N
        }
    }
}
*/
// *4
// read from C_N, write into X_N
/*
ancien
void modem_BPSK_modulate(const uint8_t *C_N, int32_t *X_N, size_t N) {
    
    int32x4_t v_one = vdupq_n_s32(1);  
    int32x4_t v_two = vdupq_n_s32(2);  

    for (size_t i = 0; i <= N; i += 4) {
        // Charger 4 valeurs de C_N dans un registre int32
        int32x4_t c = {C_N[i], C_N[i+1], C_N[i+2], C_N[i+3]};
        // Calculer 1 - 2*c  donne  +1 si c=0,  -1 si c=1
        int32x4_t x = vsubq_s32(v_one, vmulq_s32(v_two, c));
        // Stocker dans X_N
        vst1q_s32(X_N + i, x);
    }
}

/*
void modem_BPSK_modulate(const uint8_t *C_N, int32_t *X_N, size_t N) {
    int32x4_t v_one = vdupq_n_s32(1);
    size_t i = 0;

    // Traitement par blocs de 8 pour maximiser l'usage des registres 128 bits
    for (; i + 7 < N; i += 8) {
        // 1. Charger 8 valeurs (8 bits) d'un coup
        uint8x8_t c8 = vld1_u8(C_N + i);

        // 2. Étendre à 16 bits puis à 32 bits
        uint16x8_t c16 = vmovl_u8(c8);
        int32x4_t c32_low  = vreinterpretq_s32_u32(vmovl_u16(vget_low_u16(c16)));
        int32x4_t c32_high = vreinterpretq_s32_u32(vmovl_u16(vget_high_u16(c16)));

        // 3. Calculer 1 - (c << 1) (équivaut à 1 - 2c)
        int32x4_t x_low  = vsubq_s32(v_one, vshlq_n_s32(c32_low, 1));
        int32x4_t x_high = vsubq_s32(v_one, vshlq_n_s32(c32_high, 1));

        // 4. Stocker les résultats
        vst1q_s32(X_N + i, x_low);
        vst1q_s32(X_N + i + 4, x_high);
    }

    // 5. Gérer les éléments restants proprement (scalaire)
    for (; i < N; i++) {
        X_N[i] = 1 - (C_N[i] << 1);
    }
}*/

void modem_BPSK_modulate(const uint8_t *C_N, int32_t *X_N, size_t N) {
    int32x4_t v_one = vdupq_n_s32(1);  
    int32x4_t v_two = vdupq_n_s32(2);
    for (size_t i = 0; i < N/8; i += 16) {
        //extraire 16 octets 
        uint8x16_t bytes = vld1q_u8(C_N + i);
        // traiter chaque octet 
        for (size_t j = 0; j < 16 ; j++){
            uint8_t byte = vgetq_lanes(bytes, j);
            //extraire les bits LSB
            int32_t bits_lo[4] = {(byte >> 0) & 1, (byte >> 1) & 1, (byte >> 2) & 1, (byte >> 3) & 1};
            //extraire les bits MSB
            int32_t bits_hi[4] = {(byte >> 4) & 1, (byte >> 5) & 1, (byte >> 6) & 1, (byte >> 7) & 1};
            
            int32x4_t bits_lo = vld1q_s32(lo);
            int32x4_t bits_hi = vld1q_s32(hi);
            
            // Calculer 1 - 2*c  donne  +1 si c=0,  -1 si c=1
            int32x4_t x_lo = vsubq_s32(v_one, vmulq_s32(v_two, bits_lo));
            int32x4_t x_hi = vsubq_s32(v_one, vmulq_s32(v_two, bits_hi));
            // Stocker dans X_N
            vst1q_s32(X_N + (i+j)*8    , x_lo);
            vst1q_s32(X_N + (i+j)*8 + 4, x_hi);
        }
    }
}
