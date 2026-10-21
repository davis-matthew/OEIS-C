/*
 * A258998  a(n) = -(-1)^n if n = k^2 for positive integer k, otherwise 0.
 * https://oeis.org/A258998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 0 if n is not a perfect square; a(k²) = (-1)^(k+1) (i.e.
 *            1 for odd k, –1 for even k). a(0) = 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* Binary‑search integer square root for 0 ≤ n ≤ 2^64−1. */
    idx_t lo = 0;
    idx_t hi = 1ULL << 32;              /* 2^32 > sqrt(2^64−1) */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t r = isqrt_u64(n);
    if (r * r == n)                       /* perfect square */
        return (seq_t)((r & 1) ? 1 : -1);  /* 1 if r odd, –1 if r even */
    return 0;
}
