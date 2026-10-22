/*
 * A376017  a(n) = Sum_{d|n} d^(n/d - d) * binomial(n/d,d).
 * https://oeis.org/A376017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d <= n/d} d^(n/d - d) * binomial(n/d, d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 51, u64 n <= 107,
 *            u128 n <= 221, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;
        idx_t m = n / d;
        seq_t exp = (seq_t)(m - d);   /* exponent is non‑negative here */
        seq_t powd = big_pow((seq_t)d, exp);
        seq_t bin = binomial(m, d);
        sum += powd * bin;
    }
    return sum;
}
