/*
 * A249307  Triangle read by rows: A249095(n,k) * 2^k, k = 0 .. 2*n+1.
 * https://oeis.org/A249307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A249095(r,k) * 2^k where r = floor(sqrt(n)), k = n - r*r,
 *            and A249095(r,k) = binom(r, k/2) if k even, binom(r-1, (k-1)/2)
 *            if k odd
 * Range:     u8 n <= 21, u16 n <= 75, u32 n <= 246, u64 n <= 882,
 *            u128 n <= 3332, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

/* Integer floor square root for unsigned 64‑bit values. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = (v >> 1) + 1;    /* upper bound is tight enough */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= v / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);                 /* row number   */
    idx_t k = n - r * r;                    /* offset in row */

    seq_t bin;
    if ((k & 1) == 0) {                     /* even k      */
        idx_t half = k >> 1;
        bin = binom(r, half);
    } else {                                /* odd k       */
        idx_t half = (k - 1) >> 1;
        bin = binom(r - 1, half);
    }

    seq_t powk = pow2(k);                   /* 2^k         */
    return bin * powk;                      /* final value */
}
