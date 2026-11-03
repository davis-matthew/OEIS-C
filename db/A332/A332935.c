/*
 * A332935  Sum of ceiling(n^(3/2)) where d runs through the divisors of n.
 * https://oeis.org/A332935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} ceil(d^(3/2))
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1008, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9974 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

static inline seq_t ceil_sqrt_cubed(idx_t d)
{
    seq_t dd = (seq_t)d;
    seq_t t  = dd * dd * dd;          /* d^3 */
    return floor_sqrt(t - 1) + 1;     /* ceil(sqrt(d^3)) = ceil(d^(3/2)) */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)              /* iterate divisors up to sqrt(n) */
    {
        if (n % d == 0)
        {
            sum += ceil_sqrt_cubed(d);
            idx_t e = n / d;
            if (e != d)                             /* add paired divisor if distinct */
                sum += ceil_sqrt_cubed(e);
        }
    }
    return sum;
}
