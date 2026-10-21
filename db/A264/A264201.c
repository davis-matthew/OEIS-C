/*
 * A264201  Numerator of sum of numbers in set g(n) generated as in Comments.
 * https://oeis.org/A264201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) + 9*a(n-2) + 18*a(n-3) 
 *                    - 81*a(n-4) - 162*a(n-5) - 243*a(n-6),
 *          a(0)=0, a(1)=1, a(2)=7, a(3)=46, a(4)=265, a(5)=1519, a(6)=8560
 * The sequence gives the numerator of the sum of numbers in the recursively defined set g(n);
 * this recurrence is valid for all n ≥ 6.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of stored terms (cache_size = last_index+1) */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* Initialize cache with the first 7 terms */
    if (cache == NULL) {
        cache_cap  = 7;
        cache      = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)0;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)7;   /* a(2) */
        cache[3] = (seq_t)46;  /* a(3) */
        cache[4] = (seq_t)265; /* a(4) */
        cache[5] = (seq_t)1519;/* a(5) */
        cache[6] = (seq_t)8560;/* a(6) */
        cache_size = 7;        /* terms 0 .. 6 available */
    }

    /* If requested term already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Ensure enough capacity for new terms */
    idx_t need = n + 1;
    if (need > cache_cap) {
        idx_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 7;
        while (new_cap < need)
            new_cap <<= 1;         /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t pos = 4 * cache[i-1] + 9 * cache[i-2] + 18 * cache[i-3];
        seq_t neg = 81 * cache[i-4] + 162 * cache[i-5] + 243 * cache[i-6];
        cache[i] = pos - neg;
    }
    cache_size = n + 1;   /* update number of stored terms */

    return cache[n];
}
