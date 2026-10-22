/*
 * A360074  a(n) is the greatest divisor of n divisible by the sum of its own
 *          digits.
 * https://oeis.org/A360074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the greatest divisor d of n such that d % digit_sum(d) ==
 *            0
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t best = 1;
    idx_t i;

    /* Loop over the divisors by checking i and n/i */
    for (i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            seq_t d1 = (seq_t)i;
            seq_t ds1 = digit_sum(d1);
            if (ds1 != 0 && d1 % ds1 == 0 && d1 > best)
                best = d1;

            /* complementary divisor n/i, avoid double-count on perfect squares */
            idx_t j = n / i;
            if (j != i)
            {
                seq_t d2 = (seq_t)j;
                seq_t ds2 = digit_sum(d2);
                if (ds2 != 0 && d2 % ds2 == 0 && d2 > best)
                    best = d2;
            }
        }
    }
    return best;
}
