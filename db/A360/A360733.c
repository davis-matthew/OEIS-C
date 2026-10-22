/*
 * A360733  Expansion of Sum_{k>0} (x * (1 + (k * x)^k))^k.
 * https://oeis.org/A360733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d) * binomial(d, n/d-1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 27, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A360733.
 * The function memoizes results so that any call order is supported.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Static cache to store already-computed values */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0;

    /* n starts at 1; value for n==0 is irrelevant but set to 0 for safety */
    if (n == 0) return 0;

    /* Expand the cache if needed */
    if ((size_t)n >= cacheSize) {
        size_t oldSize = cacheSize;
        cacheSize = (size_t)n + 1;              /* include index 0 */
        cache = (seq_t *)realloc(cache, cacheSize * sizeof(seq_t));

        for (idx_t i = oldSize; i <= n; ++i) {
            if (i == 0) { cache[i] = 0; continue; }

            seq_t sum = 0;
            /* iterate all divisors d of i */
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    idx_t d1 = d;
                    idx_t d2 = i / d1;

                    /* term for divisor d1 */
                    seq_t bin1 = binomial(d1, i / d1 - 1);
                    seq_t pow1 = big_pow((seq_t)d1, (seq_t)(i - d1));
                    seq_t term1 = seq_mul(bin1, pow1);
                    sum = sum + term1;

                    /* term for divisor d2 (if distinct) */
                    if (d1 != d2) {
                        seq_t bin2 = binomial(d2, i / d2 - 1);
                        seq_t pow2 = big_pow((seq_t)d2, (seq_t)(i - d2));
                        seq_t term2 = seq_mul(bin2, pow2);
                        sum = sum + term2;
                    }
                }
            }
            cache[i] = sum;
        }
    }

    return cache[n];
}
