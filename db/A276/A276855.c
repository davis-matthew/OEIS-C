/*
 * A276855  Beatty sequence for (3 + golden ratio).
 * https://oeis.org/A276855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7 + sqrt(5))/2)
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9437 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity floor(n*sqrt(5)) = floor(sqrt(5*n*n)) */
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n) {
    seq_t n_sq = (seq_t)n * (seq_t)n;            /* n^2        */
    seq_t sqrt5n = floor_sqrt((seq_t)5 * n_sq);  /* floor(n*sqrt5) */
    seq_t numer = (seq_t)7 * (seq_t)n + sqrt5n;   /* 7*n + sqrt5*n */
    return numer / (seq_t)2;                     /* floor(.../2)  */
}
