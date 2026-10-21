/*
 * A265057  Coordination sequence for (2,3,7) tiling of hyperbolic plane.
 * https://oeis.org/A265057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 124, u64 n <= 261,
 *            u128 n <= 534, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) + a(n-3) + a(n-4) + a(n-5) + a(n-6) + a(n-7)
   - a(n-9) - a(n-10),
   with a(0)=1, a(1)=3, a(2)=5, a(3)=7, a(4)=9, a(5)=12, a(6)=16,
   a(7)=20, a(8)=24, a(9)=28, a(10)=33. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 11 terms known from the sequence. */
    static const seq_t init[11] = {
        1, 3, 5, 7, 9, 12, 16, 20, 24, 28, 33
    };

    /* Cache to memoize computed values. */
    static seq_t *cache = NULL;   /* Array of computed a(n) */
    static size_t cache_cap = 0;  /* Capacity of the cache */
    static size_t cache_size = 0; /* Number of valid elements in cache */

    /* For n < 11 we can return the hard‑coded value. */
    if (n < 11) return init[(size_t)n];

    /* Create and initialise the cache if this is the first call. */
    if (cache == NULL) {
        /* Allocate enough space for at least the first 11 terms, or for up
           to n if n is larger. */
        cache_cap = (size_t) (n + 1);
        if (cache_cap < 11) cache_cap = 11;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory; return 0 as a safe value */

        for (size_t i = 0; i < 11; ++i) cache[i] = init[i];
        cache_size = 11;
    }

    /* Extend the cache to hold up to index n if needed. */
    if ((size_t)n + 1 > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            cache = NULL;
            cache_size = 0;
            cache_cap = 0;
            return 0; /* out of memory */
        }
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Compute missing values up to n using the recurrence. */
    for (size_t k = cache_size; k <= (size_t)n; ++k) {
        seq_t val = -cache[k - 1];
        val += cache[k - 3];
        val += cache[k - 4];
        val += cache[k - 5];
        val += cache[k - 6];
        val += cache[k - 7];
        val -= cache[k - 9];
        val -= cache[k - 10];
        cache[k] = val;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
