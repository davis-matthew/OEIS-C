/*
 * A332935  Sum of ceiling(n^(3/2)) where d runs through the divisors of n.
 * https://oeis.org/A332935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} ceil(d^(3/2))
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1008, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

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
