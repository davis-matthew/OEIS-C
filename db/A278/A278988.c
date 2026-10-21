/*
 * A278988  a(n) is the number of words of length n over an alphabet of size 3
 *          that are in standard order and which have the property that every
 *          letter that appears in the word is repeated.
 * https://oeis.org/A278988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1)-40*a(n-2)+82*a(n-3)-91*a(n-4)+52*a(n-5)-12*a(n-6)
 * for n > 9.  Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=4,
 * a(5)=11, a(6)=41, a(7)=162, a(8)=610, a(9)=2165.  The function
 * remembers computed values so calls in any order are handled correctly. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache and its size information */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of valid elements stored */
    static idx_t cache_capacity = 0; /* allocated size of cache */

    /* Helper to enlarge cache to at least new_cap elements */
    if (cache_capacity == 0) {
        /* initial allocation with capacity 10 for the base values */
        cache_capacity = 10;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed, safe return */
        /* initialise base values up to n = 9 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)11;
        cache[6] = (seq_t)41;
        cache[7] = (seq_t)162;
        cache[8] = (seq_t)610;
        cache[9] = (seq_t)2165;
        cache_size = 10;   /* indices 0..9 filled */
    }

    /* If the requested n is already in the cache, return it directly. */
    if (n < cache_size) {
        return cache[(size_t)n];
    }

    /* Ensure the cache is large enough for indices up to n */
    if (n >= cache_capacity) {
        idx_t new_cap = cache_capacity;
        /* double until sufficient */
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed, safe return */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 9) {
            /* Base case already set, no action needed. */
            continue;
        }
        seq_t val = 10 * cache[(size_t)(i - 1)]
                  - 40 * cache[(size_t)(i - 2)]
                  + 82 * cache[(size_t)(i - 3)]
                  - 91 * cache[(size_t)(i - 4)]
                  + 52 * cache[(size_t)(i - 5)]
                  - 12 * cache[(size_t)(i - 6)];
        cache[(size_t)i] = val;
    }
    cache_size = n + 1;
    return cache[(size_t)n];
}
