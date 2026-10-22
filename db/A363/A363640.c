/*
 * A363640  Expansion of Sum_{k>0} (1/(1 - k*x^k)^4 - 1).
 * https://oeis.org/A363640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} (n/d)^d * binomial(d+3,3)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"    /* big_pow(seq_t base, seq_t exp) */
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */

static inline seq_t term_for_divisor(idx_t n, idx_t d)
{
    seq_t base   = (seq_t)n / (seq_t)d;                 /* (n/d) */
    seq_t pow_val= big_pow(base, (seq_t)d);             /* (n/d)^d */
    seq_t bin_val= binomial((idx_t)d + 3, 3);           /* C(d+3,3) */
    return pow_val * bin_val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                               /* not used, offset is 1 */
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i)                        /* loop over divisors up to sqrt(n) */
    {
        if (n % i == 0)
        {
            sum += term_for_divisor(n, i);
            idx_t d2 = n / i;                          /* complementary divisor */
            if (d2 != i)
                sum += term_for_divisor(n, d2);
        }
    }
    return sum;
}
