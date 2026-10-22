/*
 * A363662  a(n) = Sum_{d|n} (n/d)^n * binomial(d+n,n).
 * https://oeis.org/A363662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^n * binomial(d+n,n) [offset 1]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t root = floor_sqrt_u64(n);
    seq_t sum = 0;

    for (idx_t d = 1; d <= root; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;

            seq_t term1 = big_pow((seq_t)(n / d1), (seq_t)n) * binomial(d1 + n, n);
            sum += term1;

            if (d1 != d2)
            {
                seq_t term2 = big_pow((seq_t)(n / d2), (seq_t)n) * binomial(d2 + n, n);
                sum += term2;
            }
        }
    }
    return sum;
}
