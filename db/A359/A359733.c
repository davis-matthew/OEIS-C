/*
 * A359733  a(n) = (1/2) * Sum_{d|n} (2*d)^(n/d).
 * https://oeis.org/A359733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * Sum_{d|n} (2*d)^(n/d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i != 0)
            continue;

        /* First divisor d = i */
        seq_t base  = (seq_t)i * 2;
        seq_t exp   = (seq_t)(n / i);
        sum += big_pow(base, exp);

        /* Second divisor d = n / i, if distinct */
        idx_t d2 = n / i;
        if (d2 != i)
        {
            seq_t base2 = (seq_t)d2 * 2;
            seq_t exp2  = (seq_t)(n / d2);   /* equals i */
            sum += big_pow(base2, exp2);
        }
    }
    /* Divide by 2 – the sum is always even for the given formula */
    return sum >> 1;
}
