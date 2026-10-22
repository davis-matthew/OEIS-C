/*
 * A344490  a(n) = 1 + Sum_{k=0..n-3} binomial(n-2,k) * a(k).
 * https://oeis.org/A344490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-3} binomial(n-2,k) * a(k) with initial
 *            values a(0) = a(1) = a(2) = 1. This implementation memoises
 *            previous results in a static array that grows as needed. It is
 *            compiled for both C11 and C++17: for C++ we use operator
 *            new/delete to correctly construct the (possibly non‑tri...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* if we already have the value, just return it */
    if (n < cache_len)
        return cache[n];

    /* expand the cache to at least length n+1 */
    idx_t new_len = n + 1;
    seq_t *new_cache = NULL;

#if defined(__cplusplus)
    /* C++: allocate a new array and copy old contents */
    new_cache = new seq_t[new_len];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    /* free old array if it existed */
    if (cache != NULL)
        delete[] cache;
#else
    /* C: realloc or malloc */
    new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();  /* allocation failure */
#endif

    /* compute new entries from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i < 3) {
            new_cache[i] = (seq_t)1;
            continue;
        }
        idx_t m = i - 2;              /* binomial upper index */
        seq_t C = (seq_t)1;           /* C(m,0) = 1 */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= i - 3; ++k)
        {
            sum += C * new_cache[k];
            /* update C to C(m, k+1) */
            seq_t numer = C * (seq_t)(m - k);
            seq_t denom = (seq_t)(k + 1);
            C = numer / denom;
        }
        new_cache[i] = (seq_t)1 + sum;
    }

    cache = new_cache;
    cache_len = new_len;
    return cache[n];
}
