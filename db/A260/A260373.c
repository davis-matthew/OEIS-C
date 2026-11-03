/*
 * A260373  The nearest perfect square to n!
 * https://oeis.org/A260373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = square of the nearest integer to sqrt(n!) (A055227(n)^2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t compute_factorial(idx_t n)
{
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        f = mul_small(f, (unsigned int)i);
    return f;
}

seq_t a(idx_t n)
{
    seq_t fact = compute_factorial(n);

    seq_t s = floor_sqrt(fact);          /* floor(sqrt(n!))          */
    seq_t sq1 = seq_mul(s, s);           /* s^2                      */
    seq_t sp1 = s + (seq_t)1;            /* s + 1                    */
    seq_mul(sp1, sp1);                  /* reuse result as sq2       */
    seq_t sq2 = seq_mul(sp1, sp1);       /* (s+1)^2                  */

    seq_t diff1 = fact - sq1;            /* fact - s^2               */
    seq_t diff2 = sq2 - fact;            /* (s+1)^2 - fact           */

    return (diff1 <= diff2) ? sq1 : sq2;
}
