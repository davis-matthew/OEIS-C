/*
 * A338685  a(n) = Sum_{d|n} d^n * binomial(d, n/d).
 * https://oeis.org/A338685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^n * binomial(d, n/d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t t = big_pow((seq_t)d, (seq_t)n);
            t = t * binomial(d, n / d);
            sum += t;
        }
    }
    return sum;
}
