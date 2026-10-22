/*
 * A369592  Expansion of (1/x) * Series_Reversion( x / (1+x+x^4/(1+x)^3) ).
 * https://oeis.org/A369592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 47, u64 n <= 88, u128 n <= 160,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (1/(n+1)) * Sum_{k=0..⌊n/4⌋} (n-4k+1) * binomial(n+1,k)
 * (OEIS A369592)
 * 
 * The function uses a 128‑bit intermediate for seq_t sizes up to 64 bits
 * to avoid overflow, and falls back on the arbitrary‑precision binomial
 * routine for larger seq_t types.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Fast path for seq_t that fits in 64 bits. */
    if (sizeof(seq_t) <= 8) {
        idx_t maxk = n / 4;
        __uint128_t sum128 = 0;
        __uint128_t binom128 = 1;          /* binomial(n+1,0) */
        for (idx_t k = 0; k <= maxk; ++k) {
            __uint128_t factor = ( __uint128_t)n - ( __uint128_t)(4 * k) + 1;
            sum128 += factor * binom128;
            if (k < maxk) {
                __uint128_t num = ( __uint128_t)(n + 1) - ( __uint128_t)k;
                __uint128_t den = ( __uint128_t)(k + 1);
                binom128 = (binom128 * num) / den;   /* binomial(n+1, k+1) */
            }
        }
        return (seq_t)(sum128 / ( (__uint128_t)n + 1 ));
    }

    /* Fallback for arbitrary‑precision seq_t types. */
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t binom = binomial(n + 1, k);          /* arbitrary precision */
        seq_t factor = (seq_t)n - 4 * (seq_t)k + 1;
        sum += binom * factor;
    }
    return sum / ((seq_t)n + 1);
}
