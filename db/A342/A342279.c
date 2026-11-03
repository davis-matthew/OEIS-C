/*
 * A342279  A bisection of A000201: a(n) = A000201(2*n+1).
 * https://oeis.org/A342279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n + 1) * φ) where φ = (1 + √5) / 2
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    = floor(( (2*n + 1) + floor((2*n + 1) * √5) ) / 2) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* k = 2*n + 1  (odd index in A000201) */
    seq_t k = (seq_t)2 * (seq_t)n + (seq_t)1;

    /* Compute s = floor(k * √5) = floor(√(5 * k²)) */
    seq_t k_sq     = seq_mul(k, k);               /* k * k   */
    seq_t five_k_sq = seq_mul(k_sq, (seq_t)5);   /* 5 * k²  */
    seq_t s        = floor_sqrt(five_k_sq);       /* floor(√(5*k²)) */

    /* a(n) = floor((k + s) / 2) */
    seq_t sum = k + s;
    seq_t result = sum / (seq_t)2;

    return result;
}
