/*
 * A301291  Expansion of (x^4+3*x^3+x^2+3*x+1)/((x^2+1)*(x-1)^2).
 * https://oeis.org/A301291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) (n > 4) Initial
 *            values: a(0)=1, a(1)=5, a(2)=9, a(3)=13, a(4)=18
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for computed values */
    static seq_t *cache = NULL;   /* cache[i] holds a(i) */
    static size_t cap   = 0;      /* allocated size of cache      */
    static size_t last  = 4;      /* highest index already computed */

    /* Ensure the cache can hold the entry for n */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t newcap = cap ? cap * 2 : 5;
        if (newcap < 5)       newcap = 5;
        if (newcap < needed)  newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;

        /* Initialise the first five entries if they are now allocated */
        if (cap < 1) cache[0] = (seq_t)1;
        if (cap < 2) cache[1] = (seq_t)5;
        if (cap < 3) cache[2] = (seq_t)9;
        if (cap < 4) cache[3] = (seq_t)13;
        if (cap < 5) cache[4] = (seq_t)18;

        cap = newcap;
    }

    /* If the needed term is already computed, just return it */
    if (n <= last)
        return cache[(size_t)n];

    /* Compute missing terms using the recurrence */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        cache[i] = 2 * cache[i-1] - 2 * cache[i-2] + 2 * cache[i-3] - cache[i-4];
    }
    last = (size_t)n;
    return cache[(size_t)n];
}
