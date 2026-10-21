/*
 * A246478  Number of length n+3 0..7 arrays with no pair in any consecutive
 *          four terms totalling exactly 7.
 * https://oeis.org/A246478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2) + 5*a(n-3) + a(n-4) Offset: 1 (a(1) =
 *            1880)
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the A246478 sequence value using memoised recurrence. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms currently stored */

    /* guard against n == 0; offset is 1 */
    if (n == 0)
        return (seq_t)0;

    /* Initial terms (n = 1..4) */
    static const seq_t init[4] = {1880, 10376, 57416, 317576};

    /* First call: allocate 4 slots and initialise */
    if (!cache) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure */
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init[i];
    }

    /* If request is within cached range, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow cache to n terms */
    size_t old_len = cache_len;
    cache_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;               /* allocation failure */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (size_t i = old_len; i < cache_len; ++i) {
        /* i corresponds to term number i+1 (1‑based) */
        cache[i] = (seq_t)5 * cache[i - 1]          /* a(i)      */
                 + (seq_t)2 * cache[i - 2]          /* a(i-1)    */
                 + (seq_t)5 * cache[i - 3]          /* a(i-2)    */
                 +                      cache[i - 4];/* a(i-3)    */
    }

    return cache[n - 1];
}
