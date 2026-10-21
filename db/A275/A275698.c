/*
 * A275698  a(0) = 2, after that a(n) is 3 plus the least common multiple of
 *          previous terms.
 * https://oeis.org/A275698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2; a(1) = 5; Recurrence: a(n) = 3 + a(n-1)*(a(n-1)-3) for
 *            n >= 2 This implements the OEIS sequence A275698
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored (0..cache_len-1) */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* ensure we have at least two initial values */
    if (cache == NULL)
    {
        cache_cap = 2;
        cache_len = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory: return default */
        cache[0] = 2;                /* a(0) */
        cache[1] = 5;                /* a(1) */
    }

    /* if n is already computed, return it */
    if ((idx_t)n < cache_len)
        return cache[n];

    /* grow the cache to accommodate up to n (inclusive) */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap)
    {
        /* double until we exceed needed capacity */
        size_t cap = cache_cap;
        while (cap < new_cap) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* recurrence: a(i) = 3 + a(i-1)*(a(i-1)-3) */
        seq_t prev = cache[i - 1];
        seq_t next = prev * (prev - (seq_t)3) + (seq_t)3;
        cache[i] = next;
    }
    cache_len = n + 1; /* now have all up to n */

    return cache[n];
}
