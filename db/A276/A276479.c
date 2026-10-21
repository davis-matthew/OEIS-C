/*
 * A276479  a(n) = floor(Sum_{d|n} 0.d).
 * https://oeis.org/A276479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor( Σ_{d|n} d / 10^{digits(d)} )
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Compute the sum of decimal fractions 0.d for all divisors d of n,
   then return its integer part.) */

#include <stdint.h>
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset = 1). For safety, handle n == 0. */
    if (n == 0)
        return 0;

    /* Determine the maximum digit length among all divisors:
       it is the digit length of n itself. */
    int max_digits = digits(n);

    /* Compute 10^max_digits as a seq_t value. */
    seq_t pow10 = (seq_t)1;
    for (int i = 0; i < max_digits; ++i)
        pow10 *= (seq_t)10;

    seq_t sum = (seq_t)0;

    /* Enumerate divisors by iterating i up to sqrt(n). */
    for (idx_t i = (idx_t)1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* Process divisor i. */
            int di = digits(i);
            int diff1 = max_digits - di;
            seq_t mult1 = (seq_t)1;
            for (int k = 0; k < diff1; ++k)
                mult1 *= (seq_t)10;
            sum += (seq_t)i * mult1;

            /* Complementary divisor j = n / i. */
            idx_t j = n / i;
            if (j != i)
            {
                int dj = digits(j);
                int diff2 = max_digits - dj;
                seq_t mult2 = (seq_t)1;
                for (int k = 0; k < diff2; ++k)
                    mult2 *= (seq_t)10;
                sum += (seq_t)j * mult2;
            }
        }
    }

    /* floor of the sum is just integer division by pow10. */
    return sum / pow10;
}
