/*
 * A252630  Numbers n such that the sum of the hexagonal numbers X(n), X(n+1),
 *          X(n+2) and X(n+3) is equal to the heptagonal number H(m) for some
 *          m.
 * https://oeis.org/A252630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 323*a(n-1) - 323*a(n-2) + a(n-3);
   a(1)=50, a(2)=16503, a(3)=5314316 */

#include <stdlib.h>

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* capacity of the allocated array */

    /* Initialise cache with the first three terms on first call */
    if (cache_len == 0) {
        /* Start with a small capacity; will grow if needed */
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* If allocation fails we abort – the build system will handle it */
        if (!cache) abort();

        cache[0] = (seq_t)50;          /* a(1) */
        cache[1] = (seq_t)16503;       /* a(2) */
        cache[2] = (seq_t)5314316;     /* a(3) */
        cache_len = 3;
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache capacity if necessary to reach index n */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* Compute subsequent terms up to index n using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t last  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t prev3 = cache[i - 3];

        seq_t term = ((seq_t)323 * last) - ((seq_t)323 * prev2) + prev3;
        cache[i] = term;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
