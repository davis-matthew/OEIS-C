/*
 * A280351  Expansion of Sum_{k>=0} (x/(1 - x))^(k^3).
 * https://oeis.org/A280351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n^(1/3))} binomial(n-1, k^3-1) (a(0) = 1)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t kmax = int_cube_root(n);
    seq_t sum = 0;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        idx_t k3 = k * k * k;          /* k^3, fits in idx_t (k <= 21 for n <= 10000) */
        idx_t r  = k3 - 1;              /* r = k^3 - 1 <= n-1 */
        sum += binomial(n - 1, r);
    }

    return sum;
}
