/*
 * A264564  Number of (n+1)X(3+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change 1,0 1,1 0,-1 or -1,1.
 * https://oeis.org/A264564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)+a(n-2)+6*a(n-3)-6*a(n-4)+6*a(n-5)-27*a(n-6)+5*a(n-7)-9*a(n-8)+33*a(n-9)-4*a(n-10)+7*a(n-11)-12*a(n-12)+a(n-13)-a(n-14)+a(n-15)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding all computed terms up to the largest n requested */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of indices allocated (0 … cache_len-1) */

    /* initial terms for indices 1 … 15 (index 0 is unused) */
    static const seq_t init[16] = {
        (seq_t)0,
        (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)31,  (seq_t)79,
        (seq_t)224, (seq_t)646, (seq_t)1784, (seq_t)5010,
        (seq_t)14026, (seq_t)39283, (seq_t)110214, (seq_t)308476,
        (seq_t)864321, (seq_t)2421872
    };

    /* First time: allocate space for indices 0 … 15 */
    if (cache == NULL) {
        cache_len = 16;                     /* reserve space for 0‑15 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* If the requested index is already known, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Need to grow the cache to include index n (i.e. size n+1) */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_len = new_len;

    /* Compute all missing terms from old_len up to n */
    for (idx_t i = (idx_t)old_len; i <= n; ++i)
    {
        /* The recurrence is valid only for i >= 16, but old_len is >= 16
           whenever we reach this loop, so the indices below are always valid. */
        seq_t val = 
              2*cache[(size_t)(i-1)] 
            + cache[(size_t)(i-2)] 
            + 6*cache[(size_t)(i-3)] 
            - 6*cache[(size_t)(i-4)] 
            + 6*cache[(size_t)(i-5)] 
            - 27*cache[(size_t)(i-6)] 
            + 5*cache[(size_t)(i-7)] 
            - 9*cache[(size_t)(i-8)] 
            + 33*cache[(size_t)(i-9)] 
            - 4*cache[(size_t)(i-10)] 
            + 7*cache[(size_t)(i-11)] 
            - 12*cache[(size_t)(i-12)] 
            + cache[(size_t)(i-13)] 
            - cache[(size_t)(i-14)] 
            + cache[(size_t)(i-15)];
        cache[(size_t)i] = val;
    }

    return cache[n];
}
