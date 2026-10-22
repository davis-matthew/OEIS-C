/*
 * A338661  a(n) = Sum_{d|n} d^n * binomial(d+n/d-2, d-1).
 * https://oeis.org/A338661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^n * binomial(d + n/d - 2, d-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0)                /* offset is 1; 0 is unused */
        return 0;

    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
#if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t *>(realloc(cache,
                    new_size * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t *)realloc(cache,
                    new_size * sizeof(seq_t));
#endif
        if (!tmp) abort();
        /* initialise new entries to zero */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Return memoised value if present */
    if (cache[n] != 0)
        return cache[n];

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        /* contribution from divisor d1 */
        seq_t pow1  = big_pow((seq_t)d1, (seq_t)n);
        seq_t bin1  = binomial(d1 + n/d1 - 2, d1 - 1);
        sum += pow1 * bin1;

        /* If d2 is different, add its contribution too */
        if (d2 != d1) {
            seq_t pow2  = big_pow((seq_t)d2, (seq_t)n);
            seq_t bin2  = binomial(d2 + n/d2 - 2, d2 - 1);
            sum += pow2 * bin2;
        }
    }

    cache[n] = sum;
    return sum;
}
