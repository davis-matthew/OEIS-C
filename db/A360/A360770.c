/*
 * A360770  Expansion of Sum_{k>0} (x * (k + x^k))^k.
 * https://oeis.org/A360770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d - n/d + 1) * binomial(d, n/d - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i != 0)
            continue;

        /* divisor d = i */
        idx_t d = i;
        idx_t m = n / d;
        idx_t k = m - 1;          /* n/d - 1 */
        if (k <= d)
        {
            seq_t bin = binomial(d, k);
            seq_t pow_val = big_pow((seq_t)d, (seq_t)(d - m + 1));
            sum += bin * pow_val;
        }

        /* divisor d2 = n / i (different from d) */
        idx_t d2 = n / i;
        if (d2 != d)
        {
            d = d2;
            m = n / d;
            k = m - 1;
            if (k <= d)
            {
                seq_t bin = binomial(d, k);
                seq_t pow_val = big_pow((seq_t)d, (seq_t)(d - m + 1));
                sum += bin * pow_val;
            }
        }
    }
    return sum;
}
