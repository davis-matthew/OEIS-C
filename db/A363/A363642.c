/*
 * A363642  Expansion of Sum_{k>0} x^k/(1 - k*x^k)^3.
 * https://oeis.org/A363642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^{d-1} * binomial(d+1,2)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure enough space for index n (1‑based). */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed.  All sequence values are
       positive, so a 0 value indicates “not yet computed”. */
    if (cache[n - 1] != 0)
        return cache[n - 1];

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* Process divisor d */
            seq_t factor  = (seq_t)(n / d);   /* n/d */
            seq_t exp     = (seq_t)(d - 1);   /* d-1 */
            seq_t pow_term = big_pow(factor, exp);
            seq_t binom = ( (seq_t)(d + 1) * (seq_t)d ) / 2;
            sum += pow_term * binom;

            /* Process paired divisor if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                factor  = (seq_t)(n / d2);     /* n/d2  == d */
                exp     = (seq_t)(d2 - 1);
                pow_term = big_pow(factor, exp);
                binom = ( (seq_t)(d2 + 1) * (seq_t)d2 ) / 2;
                sum += pow_term * binom;
            }
        }
    }

    cache[n - 1] = sum;
    return sum;
}
