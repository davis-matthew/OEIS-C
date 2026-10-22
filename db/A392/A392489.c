/*
 * A392489  a(n) = Sum_{k=0..floor(n/2)} binomial(k+1,2*n-4*k+1).
 * https://oeis.org/A392489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 38, u32 n <= 74, u64 n <= 145,
 *            u128 n <= 289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=0, a(2)=2, a(3)=0, a(4)=3,
   a(n)=2*a(n-2)-a(n-4)+a(n-5)   for n≥5 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static idx_t next = 0;         /* first index not yet computed */

    /* Initialise with the first five terms */
    if (cache == NULL) {
        cache_cap = 8;                 /* start with a little extra space */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed – return 0 */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 0;
        cache[4] = 3;
        next = 5;                       /* next index to compute */
    }

    /* Grow cache if the requested index is outside the current range */
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)n >= (idx_t)new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failed – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = next; i <= n; ++i) {
        if (i < 5) {
            /* values 0..4 are already initialised */
            continue;
        }
        /* recurrence: a(i) = 2*a(i-2) - a(i-4) + a(i-5) */
        seq_t val = 2 * cache[i - 2];
        val -= cache[i - 4];
        val += cache[i - 5];
        cache[i] = val;
    }
    next = n + 1;                       /* we have computed up to n inclusive */

    return cache[n];
}
