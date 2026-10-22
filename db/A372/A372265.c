/*
 * A372265  a(n) = floor((2*n - 3 + sqrt(1 + 4*n!))/2).
 * https://oeis.org/A372265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n - 3 + sqrt(1 + 4*n!))/2)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   factorial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n >= 1.  The implementation uses
 * a fast 128‑bit factorial and square‑root when seq_t is 64‑bit,
 * and falls back to the arbitrary‑precision helpers otherwise. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/floor_sqrt.h"

/* Integer square root of a 128‑bit unsigned integer. */
static inline unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);  /* 2^64 is > sqrt(2^128) */
    unsigned __int128 mid, best = 0;

    while (low <= high) {
        mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* 64‑bit implementation using unsigned __int128 for exactness. */
    if (sizeof(seq_t) <= 8) {
        /* Compute n! in __int128.  For n beyond ~30 the result
         * would overflow __int128, but a(n) also overflows 64‑bit
         * before that, so the calling program only uses this
         * path for small n where the result fits seq_t. */
        unsigned __int128 f = 1;
        for (idx_t i = 2; i <= n; ++i) {
            f *= i;
        }
        unsigned __int128 val   = ((unsigned __int128)4 * f) + 1;
        unsigned __int128 sqrt_v = isqrt_u128(val);
        unsigned __int128 numerator = ((unsigned __int128)2 * n) - 3 + sqrt_v;
        seq_t result = (seq_t)(numerator / 2);
        return result;
    }

    /* Arbitrary‑precision implementation (big integer seq_t). */
    seq_t nf = factorial(n);                 /* n!  */
    seq_t val = nf * (seq_t)4 + (seq_t)1;    /* 1 + 4*n! */
    seq_t sqrt_v = floor_sqrt(val);          /* floor sqrt */
    seq_t numerator = (seq_t)2 * (seq_t)n - (seq_t)3 + sqrt_v;
    return numerator / (seq_t)2;             /* floor division */
}
