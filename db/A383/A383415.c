/*
 * A383415  Antidiagonal sums of A383414.
 * https://oeis.org/A383415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (29*4^(n+1) - 24*n - 53)/9
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 7; a(n) = 4*a(n-1) + 8*n + 7 for n ≥ 1 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache to enable memoisation of arbitrary call order. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* Allocated slots */
    static size_t cache_len = 0;  /* Next index to fill */

    /* If this is the first call, initialise seed value a(0)=7 */
    if (cache_cap == 0) {
        cache_cap = 4;                     /* small initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* abort on allocation failure */
        cache[0] = (seq_t)7;
        cache_len = 1;
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
            if (new_cap == 0) new_cap = (size_t)n + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to index n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* a(i) = 4*a(i-1) + 8*i + 7 */
        seq_t prev = cache[i - 1];
        seq_t term = (prev * (seq_t)4) + ((seq_t)8 * (seq_t)i) + (seq_t)7;
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
