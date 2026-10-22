/*
 * A366509  a(n) is the maximum number of dots on the slope of a Ferrers
 *          diagram of a partition of n into distinct parts.
 * https://oeis.org/A366509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum length of a 45° slope in any Ferrers diagram of a
 *            partition of n into distinct parts. Algorithm is a constructive
 *            search bounded by O(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square‑root for uint64_t */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;      /* avoid underflow */
            hi = mid - 1;
        }
    }
    return hi;                         /* floor(sqrt(x)) */
}

/* A123578(n): largest r with r*(r+1)/2 <= n */
static idx_t A123578_u64(idx_t n)
{
    if (n == (idx_t)-1) return 0;      /* safety for negative n in callers */
    idx_t v = 8 * n + 1;
    idx_t s = isqrt_u64(v);
    return (s - 1) >> 1;               /* (s-1)/2 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* defined only for n>=1 */

    idx_t r   = A123578_u64(n);        /* r = A123578(n) */
    idx_t tri = r * (r + 1) / 2;       /* triangular number T(r) */

    if (n == tri)                     /* n is triangular → max slope = r */
        return (seq_t)r;

    /* Search for maximum slope s from r down to 1 */
    for (idx_t s = r; s >= 1; --s) {
        for (idx_t m = s; ; ++m) {    /* top row length m (m >= s) */
            idx_t sum_min = s * m - s * (s - 1) / 2;
            if (sum_min > n) break;   /* further m only increase sum_min */

            idx_t t      = m - s;
            idx_t sum_max = sum_min + t * (t + 1) / 2;
            if (n <= sum_max)
                return (seq_t)s;      /* maximum slope found */
        }
    }

    return (seq_t)1;                  /* fallback – should never happen */
}
