/*
 * A298025  Partial sums of A298024.
 * https://oeis.org/A298025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) Initial
 *            values: a(0)=1, a(1)=5, a(2)=15, a(3)=29, a(4)=47
 * Range:     u8 n <= 9, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache      = NULL;
    static idx_t  cache_cap  = 0;          /* allocated size */
    static idx_t  cache_len  = 0;          /* number of terms computed (next free index) */

    /* Initialize cache on first call */
    if (cache == NULL) {
        /* Allocate initial space, enough for the first few terms */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely in tests */

        /* Set the base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)29;
        cache[4] = (seq_t)47;

        cache_len = 5;                /* indices 0..4 computed */
    }

    /* Grow cache if needed */
    if ((size_t)(n + 1) > (size_t)cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache     = tmp;
            cache_cap = (idx_t)new_cap;
        } /* else keep old capacity – unlikely to happen for n <= 10000 */
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 4) {
            /* These values are already set in the initial block */
            continue;
        }
        cache[i] = (seq_t)2 * cache[i-1]
                   - cache[i-2]
                   + cache[i-3]
                   - (seq_t)2 * cache[i-4]
                   + cache[i-5];
    }
    cache_len = n + 1;  /* update number of computed terms */

    return cache[n];
}
