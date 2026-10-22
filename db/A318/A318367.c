/*
 * A318367  a(n) = Sum_{d|n} (-1)^(n/d+1)*d*prime(d).
 * https://oeis.org/A318367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1)*d*prime(d)
 * Range:     u8 n <= 9, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 as a safe default. */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t positive = (seq_t)0;
    seq_t negative = (seq_t)0;

    for (idx_t i = (idx_t)1; i * i <= n; ++i)
    {
        if (n % i == (idx_t)0)
        {
            /* First divisor d1 = i */
            idx_t d1 = i;
            seq_t term1 = (seq_t)d1 * (seq_t)get_prime(d1);
            if ((n / d1) % (idx_t)2 == (idx_t)1) /* n/d1 odd -> positive sign */
                positive += term1;
            else
                negative += term1;

            /* Second divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                seq_t term2 = (seq_t)d2 * (seq_t)get_prime(d2);
                if ((n / d2) % (idx_t)2 == (idx_t)1)
                    positive += term2;
                else
                    negative += term2;
            }
        }
    }

    /* The term with d = n is always positive, so positive >= negative. */
    return positive >= negative ? positive - negative : (seq_t)0;
}
