/*
 * A365570  Expansion of e.g.f. 1 / (6 - 5 * exp(x))^(4/5).
 * https://oeis.org/A365570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: a(n) = 4*a(n-1) - 6*Sum_{k=1..n-1} (-1)^k * binomial(n-1,k) * a(n-k)
 *  Offset: 0   (a(0) is the first term)
 *  Uses memoisation and the provided binomial function.
 *  Complexity: O(n^2) additions and multiplications, which is easily fast
 *  enough for n up to 10 000 and works for all supported seq_t sizes.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/ensure_capacity.h"   /* for ensure_capacity */
#include "../utils/binomial.h"          /* for binomial */

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* if result already cached, return it */
    if (n < (idx_t)cache_len)
        return cache[n];

    /* ensure cache storage */
    if (cache_cap <= (size_t)n + 1)
        ensure_capacity((size_t)n + 1, &cache_cap, (void **) &cache, sizeof(seq_t));

    /* initialise on first call */
    if (cache_len == 0) {
        cache[0] = 1;     /* a(0) = 1 */
        cache_len = 1;
    }

    /* compute new terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) continue;           /* already initialised */

        /* sum_{k=1..i-1} (-1)^k * binomial(i-1,k) * a(i-k) */
        seq_t even_sum = 0;
        seq_t odd_sum  = 0;

        for (idx_t k = 1; k < i; ++k) {
            seq_t bin = binomial(i-1, k);           /* binomial(i-1,k) */
            seq_t term = 6 * bin * cache[i - k];    /* 6 * bin * a(i-k) */

            if ((k & 1) == 0)
                even_sum += term;
            else
                odd_sum  += term;
        }

        /* a(i) = 4*a(i-1) - even_sum + odd_sum */
        seq_t ai = 4 * cache[i-1];
        ai = ai - even_sum + odd_sum;
        cache[i] = ai;
    }

    cache_len = n + 1;
    return cache[n];
}
