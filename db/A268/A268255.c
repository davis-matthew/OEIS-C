/*
 * A268255  Number of length-(n+1) 0..2 arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7, a(2)=17, a(3)=42, a(4)=106; For n>4:
 *            a(n)=7*a(n-1)-15*a(n-2)+7*a(n-3)+6*a(n-4)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0)
        return (seq_t)0;   /* undefined for this sequence */

    /* Initialise cache with the first 4 terms on first call */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)  /* allocation failed; return 0 */
            return (seq_t)0;
        cache[0] = (seq_t)7;   /* a(1) */
        cache[1] = (seq_t)17;  /* a(2) */
        cache[2] = (seq_t)42;  /* a(3) */
        cache[3] = (seq_t)106; /* a(4) */
    }

    /* Return already computed term if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache up to the requested index n */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)        /* realloc failed */
        return (seq_t)0;
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i < new_len; ++i) {
        seq_t t = (seq_t)7 * cache[i - 1];
        t += (seq_t)7 * cache[i - 3];
        t += (seq_t)6 * cache[i - 4];
        t -= (seq_t)15 * cache[i - 2];
        cache[i] = t;
    }
    cache_len = new_len;

    return cache[n - 1];
}
