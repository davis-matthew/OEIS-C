/*
 * A341249  a(n) = floor(r*floor(s*n)), where r = 2 + sqrt(2) and s = sqrt(2).
 * https://oeis.org/A341249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2+sqrt(2))*floor(sqrt(2)*n))
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;                       /* n as seq_t */
    seq_t sq   = seq_mul(sn, sn);                /* n^2          */
    seq_t dqs  = mul_small(sq, 2);               /* 2*n^2        */
    seq_t m    = floor_sqrt(dqs);                /* floor(sqrt(2)*n) */

    seq_t sqm  = seq_mul(m, m);                  /* m^2          */
    seq_t dqs2 = mul_small(sqm, 2);              /* 2*m^2        */
    seq_t f2   = floor_sqrt(dqs2);               /* floor(sqrt(2)*m) */

    return m + m + f2;                           /* 2*m + f2      */
}
