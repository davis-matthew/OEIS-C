/*
 * A324980  a(n) = Product_{d|n} (d*sigma(d)) where sigma(k) = the sum of the
 *          divisors of k (A000203).
 * https://oeis.org/A324980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{d|n} (d * sigma(d))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t i;
    for (i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            seq_t sigma_i = sigma_of(i);
            result *= (seq_t)i * sigma_i;

            /* corresponding divisor j = n / i, if distinct */
            idx_t j = n / i;
            if (j != i)
            {
                seq_t sigma_j = sigma_of(j);
                result *= (seq_t)j * sigma_j;
            }
        }
    }
    return result;
}
