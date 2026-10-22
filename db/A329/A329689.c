/*
 * A329689  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UU, UD, HH and DU.
 * https://oeis.org/A329689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 47, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+4)*a(n) = a(n-1) - a(n-2) + 2*n*a(n-3) + (4*n-11)*a(n-4) 
   - a(n-5) - (n-6)*a(n-6).  Initial values: 
   a(0)=1, a(1)=1, a(2)=0, a(3)=1, a(4)=2, a(5)=1, a(6)=1. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t used = 0;
    static const seq_t init[7] = {1,1,0,1,2,1,1};

    /* initialise cache up to index 6 if not already */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 7);
        if (!cache) return (seq_t)0; /* allocation failed, return 0 */
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        used = 7;
    }

    /* if request is within the cache return directly */
    if ((idx_t)used > n) return cache[n];

    /* extend the cache to length n+1 */
    size_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;       /* allocation failed */
    cache = tmp;

    /* compute missing values starting from the first index >= 7 */
    size_t start = used;
    if (start < 7) start = 7;
    for (size_t k = start; k <= n; ++k) {
        /* use the recurrence formula to compute a(k) */
        seq_t pos = cache[k-1];                                   /* a(k-1)          */
        pos += (seq_t)2 * (seq_t)k * cache[k-3];                 /* 2*k*a(k-3)      */
        pos += (seq_t)(4*k-11) * cache[k-4];                     /* (4*k-11)*a(k-4) */

        seq_t neg = cache[k-2];                                   /* a(k-2)          */
        neg += cache[k-5];                                        /* a(k-5)          */
        neg += (seq_t)(k-6) * cache[k-6];                         /* (k-6)*a(k-6)    */

        seq_t numerator = pos - neg;                             /* numerator       */
        seq_t denom = (seq_t)(k + 4);                             /* denominator     */
        cache[k] = numerator / denom;                            /* a(k)            */
    }

    used = new_size;
    return cache[n];
}
