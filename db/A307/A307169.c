/*
 * A307169  First class of all proper positive solutions y1(n) = a(n) of the
 *          Pell equation x^2 - 7*y^2 = 9.
 * https://oeis.org/A307169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2) for n >= 3 Initial values: a(1) = 4,
 *            a(2) = 65
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* cache[0] unused, cache[i] holds a(i) for i >= 1 */
    static idx_t cache_size = 0;  /* highest index computed */
    static size_t cache_cap = 0;  /* current capacity of array (size_t, not idx_t) */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 2; /* accommodate indices 1 and 2 */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t)); /* +1 because index 0 unused */
        if (cache == NULL) return 0; /* allocation failure – return 0 as sentinel */
        cache[1] = 4;
        cache[2] = 65;
        cache_size = 2;
    }

    /* Grow array if needed */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n) new_cap *= 2;
        if (new_cap == cache_cap) new_cap = n + 1; /* force growth if small */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure – return 0 as sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (cache_size < n) {
        /* a(k+1) = 16*a(k) - a(k-1) */
        seq_t next = (seq_t)16 * cache[cache_size] - cache[cache_size - 1];
        cache_size++;
        cache[cache_size] = next;
    }

    return cache[n];
}
