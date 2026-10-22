/*
 * A363652  Expansion of Sum_{k>0} x^(3*k)/(1 - (k*x)^k)^3.
 * https://oeis.org/A363652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n}(n/d)^(n-3*(n/d)) * binomial(d-1,2)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 26, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Iterate over all positive divisors d of n, treating the pair
       (d , n/d) together to avoid duplicate work. */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t d1 = d;          /* first divisor */
        idx_t d2 = n / d;      /* complementary divisor */

        /* Process divisor d1 if it contributes (i.e. d1 >= 3). */
        if (d1 >= 3)
        {
            seq_t base = (seq_t)(n / d1);          /* n/d1 */
            seq_t exp  = (seq_t)n - (seq_t)3 * base;
            seq_t term = big_pow(base, exp);
            term *= binom(d1 - 1, 2);
            result += term;
        }

        /* Process divisor d2 if it is distinct from d1 and contributes. */
        if (d2 != d1 && d2 >= 3)
        {
            seq_t base = (seq_t)(n / d2);          /* n/d2 == d1 */
            seq_t exp  = (seq_t)n - (seq_t)3 * base;
            seq_t term = big_pow(base, exp);
            term *= binom(d2 - 1, 2);
            result += term;
        }
    }

    return result;
}
