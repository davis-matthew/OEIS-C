/*
 * A373254  a(n) = 1 if A003415(n) == +1 (mod 3), otherwise 0, where A003415
 *          is the arithmetic derivative.
 * https://oeis.org/A373254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A003415(n) == +1 (mod 3)]
 * Range:     u8 n <= 9277, u16 n <= 9277, u32 n <= 9277, u64 n <= 9277,
 *            u128 n <= 9277, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Edge cases: 0 and 1 have derivative 0 */
    if (n <= 1) return (seq_t)0;

    /* Factorization of n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 64;  /* capacity for distinct prime factors */
    factor_u64(n, primes, exps, &cnt);

    /* Working modulo 3 */
    uint8_t val = 1;   /* product of processed prime powers modulo 3 */
    uint8_t der = 0;   /* derivative of processed part modulo 3 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint8_t p_mod = (uint8_t)(p % 3);

        /* Compute p^e  (mod 3) */
        uint8_t val_p;
        if (p_mod == 0) {
            val_p = 0;
        } else if (p_mod == 1) {
            val_p = 1;
        } else { /* p_mod == 2 */
            val_p = (e & 1) ? 2 : 1;   /* 2^e mod 3 cycles 2,1 */
        }

        /* Compute the derivative of p^e:  D(p^e) = e * p^(e-1) */
        uint8_t der_p;
        if (e == 0) {
            der_p = 0;
        } else {
            uint8_t e_mod3 = (uint8_t)(e % 3);
            uint8_t p_pow_e_minus1;
            if (e == 1) {
                p_pow_e_minus1 = 1;  /* p^0 */
            } else {
                if (p_mod == 0) {
                    p_pow_e_minus1 = 0;
                } else if (p_mod == 1) {
                    p_pow_e_minus1 = 1;
                } else { /* p_mod == 2 */
                    p_pow_e_minus1 = ((e - 1) & 1) ? 2 : 1;  /* 2^(e-1) mod 3 */
                }
            }
            der_p = (uint8_t)((e_mod3 * p_pow_e_minus1) % 3);
        }

        /* Combine the current prime power with the accumulated product
           using D(ab) = a*D(b) + b*D(a) */
        uint8_t new_val = (uint8_t)((val * val_p) % 3);
        uint8_t new_der = (uint8_t)(((val * der_p) % 3 + (val_p * der) % 3) % 3);

        val = new_val;
        der = new_der;
    }

    /* a(n) is 1 iff the arithmetic derivative is ≡ 1 (mod 3) */
    return der == 1 ? (seq_t)1 : (seq_t)0;
}
