/*
 * A397857  G.f. A(x) satisfies A(x) = 1 + x * (1+x)^3 * A(x * (1+x)).
 * https://oeis.org/A397857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(k+3, n-k-1) * a(k) a(0) = 1
 *            (Recurrence from OEIS A397857)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* Provides binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache    = NULL;   /* array of computed values */
    static size_t cache_cap = 0;    /* allocated capacity (elements) */
    static size_t cache_len = 0;    /* number of values already computed */

    /* Ensure enough space for indices up to n */
    size_t needed = (size_t)n + 1;   /* +1 because we store a(0) .. a(n) */
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – undefined behaviour in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (size_t i = cache_len; i < needed; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else
        {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < i; ++k)
            {
                seq_t coeff   = binomial((idx_t)(k + 3), (idx_t)(i - k - 1));
                sum = sum + coeff * cache[k];
            }
            cache[i] = sum;
        }
    }
    cache_len = needed;

    return cache[n];
}
