/*
 * A387764  a(n) = Sum_{k=0..n} 2^k * binomial(2*k+1,2*n-2*k).
 * https://oeis.org/A387764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n‑1) + 8*a(n‑3) – 4*a(n‑4), a(0)=1, a(1)=2, a(2)=10,
 *            a(3)=48
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t  *cache = NULL;      /* pointer to stored terms          */
    static size_t cache_cap = 0;      /* current capacity (in terms)      */
    static idx_t  cache_len = 0;      /* number of terms stored (max idx+1) */

    /* Initialise cache with the first four terms once */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* defined first four values explicitly */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)48;
        cache_len = 4;
    }

    /* Return cached term if already computed */
    if (n < cache_len)
        return cache[n];

    /* Ensure enough capacity for terms up to index n */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        cache[i] = a1 * (seq_t)4 + a3 * (seq_t)8 - a4 * (seq_t)4;
    }

    cache_len = n + 1;
    return cache[n];
}
