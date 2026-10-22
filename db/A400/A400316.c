/*
 * A400316  G.f. A(x) satisfies A(x) = (1 + x)^2 * A(x^4 * (1 + x)), with A(0)
 *          = 1.
 * https://oeis.org/A400316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..⌊n/4⌋} binomial(k+2, n-4*k) * a(k)
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* for binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* static cache for memoised values; grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of elements allocated */

    /* initialise the cache with a(0) */
    if (cache == NULL)
    {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = (seq_t)1;
    }

    /* resize cache and compute missing values if needed */
    if (n >= cache_size)
    {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;

        for (idx_t i = old_size; i <= n; ++i)
        {
            seq_t sum = 0;
            idx_t maxk = i >> 2;        /* floor(i/4) */
            for (idx_t k = 0; k <= maxk; ++k)
            {
                idx_t r = i - 4 * k;    /* r = n-4*k */
                seq_t bin = binomial(k + 2, r);   /* returns 0 if r > k+2 */
                if (bin == (seq_t)0) continue;
                sum += bin * cache[k];
            }
            cache[i] = sum;
        }
    }

    return cache[n];
}
