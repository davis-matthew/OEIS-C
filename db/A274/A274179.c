/*
 * A274179  Expansion of f(x^1, x^6) in powers of x where f() is Ramanujan's
 *          general theta function.
 * https://oeis.org/A274179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = (7k^2+5k)/2 for some integer k, otherwise 0. The
 *            sequence contains 1 whenever the exponent (7k^2+5k)/2 occurs in
 *            Ramanujan's general theta function f(x, x^6); all other terms
 *            are 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for an unsigned 64‑bit value.
   Returns floor(sqrt(x)).  Uses binary search and avoids overflow. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x + 1;            /* hi is strictly greater than sqrt(x) */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /*  D = 56*n + 25  (always fits in idx_t for n up to ~3e16) */
    idx_t D = 56 * n + 25;
    idx_t s = isqrt_u64(D);

    /* No integer solution if D is not a perfect square. */
    if (s * s != D)
        return 0;

    /* Positive k solution.  k = (s - 5)/14 provided it is integral. */
    if (s >= 5 && (s - 5) % 14 == 0) {
        int64_t t = (int64_t)(s - 5) / 14;                 /* t ≥ 0 */
        int64_t lhs = 7 * t * t + 5 * t;                  /* 7k²+5k */
        int64_t rhs = 2 * (int64_t)n;
        if (lhs == rhs)
            return 1;
    }

    /* Negative k solution.  k = -(s + 5)/14 provided it is integral. */
    if ((s + 5) % 14 == 0) {
        int64_t t = (int64_t)(s + 5) / 14;                 /* |k| = t > 0 */
        int64_t lhs = 7 * t * t - 5 * t;                  /* 7k²+5k with k=−t */
        int64_t rhs = 2 * (int64_t)n;
        if (lhs == rhs)
            return 1;
    }

    /* No integer k satisfies the quadratic, so the coefficient is 0. */
    return 0;
}
