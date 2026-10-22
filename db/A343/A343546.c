/*
 * A343546  a(n) = n * Sum_{d|n} binomial(d+4,5)/d.
 * https://oeis.org/A343546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} binomial(d+4,5)/d
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial(d+4,5) = (d+4)(d+3)(d+2)(d+1)d / 120
   using a 128‑bit temporary to avoid overflow for d up to 10^4 */
static inline seq_t binom5(idx_t d)
{
    __uint128_t t = ( __uint128_t )(d + 4);
    t *= ( d + 3 );
    t *= ( d + 2 );
    t *= ( d + 1 );
    t *= d;
    t /= 120;
    return (seq_t)t;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            seq_t term = binom5(i);
            term *= (seq_t)(n / i);   /* (n / i) is an integer divisor */
            sum += term;

            idx_t j = n / i;
            if (j != i)
            {
                term = binom5(j);
                term *= (seq_t)(n / j); /* equals i */
                sum += term;
            }
        }
    }
    return sum;
}
