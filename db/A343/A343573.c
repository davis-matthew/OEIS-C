/*
 * A343573  a(n) = Sum_{d|n} d^d * binomial(d+n/d-2, d-1).
 * https://oeis.org/A343573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d * binomial(d + n/d - 2, d - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A343573. */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate i from 1 to sqrt(n) to find divisors */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i != 0) continue;

        /* first divisor d1 = i */
        idx_t d1 = i;
        idx_t k1 = n / d1;               /* n/d1 */

        seq_t pow1 = big_pow((seq_t)d1, (seq_t)d1);
        seq_t bin1 = binomial(d1 + k1 - 2, d1 - 1);
        sum += pow1 * bin1;

        /* second divisor d2 = n/i, if distinct */
        idx_t d2 = k1;
        if (d2 != d1)
        {
            idx_t k2 = n / d2;           /* equals i */

            seq_t pow2 = big_pow((seq_t)d2, (seq_t)d2);
            seq_t bin2 = binomial(d2 + k2 - 2, d2 - 1);
            sum += pow2 * bin2;
        }
    }
    return sum;
}
