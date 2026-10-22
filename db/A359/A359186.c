/*
 * A359186  a(n) = Sum_{d|n} d * 4^(d-1).
 * https://oeis.org/A359186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * 4^(d-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t pow1 = big_pow((seq_t)4, (seq_t)(d1 - 1));
            total += (seq_t)d1 * pow1;

            if (d1 != d2)
            {
                seq_t pow2 = big_pow((seq_t)4, (seq_t)(d2 - 1));
                total += (seq_t)d2 * pow2;
            }
        }
    }
    return total;
}
