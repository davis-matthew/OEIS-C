/*
 * A242634  G.f. A(x) satisfies A(x) = A(x^2) / (1 - x) + x / (1 - x^2).
 * https://oeis.org/A242634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, for n>1: a(n)=a(n-2)+a(floor(n/2))
 * Range:     u8 n <= 39, u16 n <= 169, u32 n <= 1187, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;

    /* Return pre‑computed value if available */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache if necessary */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* If memory allocation fails, fall back to pure recursion
           (unlikely to happen for practical inputs). */
        if (n == 0) return (seq_t)0;
        if (n == 1) return (seq_t)1;
        return a(n - 2) + a(n >> 1);
    }
    cache = tmp;

    /* Compute missing values sequentially. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else
            cache[i] = cache[i - 2] + cache[i >> 1];
    }

    cache_len = new_len;
    return cache[n];
}
