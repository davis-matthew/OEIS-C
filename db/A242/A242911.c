/*
 * A242911  Half the number of compositions of n into exactly two different
 *          parts with equal multiplicities.
 * https://oeis.org/A242911
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * Sum_{d|n} floor((d-1)/2) * binomial(2*n/d, n/d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 104,
 *            u128 n <= 200, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where binomial is the standard binomial coefficient */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3.  For values < 3 we return 0. */
    if (n < 3)
        return 0;

    seq_t sum = 0;

    /* Iterate over all positive divisors d of n. */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* first divisor */
            idx_t d1 = i;
            idx_t t1 = (d1 - 1) / 2;           /* floor((d1-1)/2)   */
            idx_t k1 = n / d1;                 /* n/d1             */
            seq_t comb1 = binom(2 * k1, k1);   /* C(2k1, k1)       */
            sum += (seq_t)t1 * comb1;

            /* second divisor if distinct */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                idx_t t2 = (d2 - 1) / 2;
                idx_t k2 = n / d2;            /* equals i          */
                seq_t comb2 = binom(2 * k2, k2);
                sum += (seq_t)t2 * comb2;
            }
        }
    }

    /* The sum is guaranteed to be even. */
    return sum / 2;
}
