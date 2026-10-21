/*
 * A291393  p-INVERT of (1,1,0,0,0,0,...), where p(S) = (1 - S)(1 - 2 S).
 * https://oeis.org/A291393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 4*a(n-3) - 2*a(n-4) (n ≥ 5) Initial
 *            values: a(0)=3, a(1)=10, a(2)=29, a(3)=83, a(4)=232
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t cache_len = 0;   /* current length of cache */

    /* If we already have the answer, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache can hold terms up to n (inclusive). */
    idx_t new_len = n + 1;
    seq_t *tmp;
#ifdef __cplusplus
    tmp = static_cast<seq_t*>(realloc(cache, new_len * sizeof(seq_t)));
#else
    tmp = realloc(cache, new_len * sizeof(seq_t));
#endif
    if (!tmp) {        /* allocation failed – return 0 as a fallback */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute any newly needed terms (starting from the current cache
       length up to new_len-1). */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0)
            cache[i] = (seq_t)3;
        else if (i == 1)
            cache[i] = (seq_t)10;
        else if (i == 2)
            cache[i] = (seq_t)29;
        else if (i == 3)
            cache[i] = (seq_t)83;
        else if (i == 4)
            cache[i] = (seq_t)232;
        else
            cache[i] = (seq_t)(3 * cache[i-1] + cache[i-2]
                               - 4 * cache[i-3] - 2 * cache[i-4]);
    }

    /* Update the cached length. */
    cache_len = new_len;

    return cache[n];
}
