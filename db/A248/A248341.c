/*
 * A248341  a(n) = 10^n - 7^n.
 * https://oeis.org/A248341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 17*a(n-1) - 70*a(n-2), a(0) = 0, a(1) = 3 */
#include <stddef.h>
#include <stdlib.h>

/* Compute a(n) = 10^n – 7^n using the two‑term recurrence, cached for speed */
seq_t a(idx_t n)
{
    /* static cache that holds previously computed values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;  /* number of stored values   */
    static size_t cache_cap = 0;  /* current capacity of cache */

    /* Ensure the cache can hold values up to index n                */
    if (cache == NULL) {
        /* first allocation – at least two slots for a(0) and a(1) */
        size_t init_cap = (n + 1 > 2) ? (n + 1) : 2;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache_len = 0;
    } else if (cache_cap < (size_t)n + 1) {
        /* grow the cache to at least n+1 slots */
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base cases if they are not yet stored */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* If the requested value is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)17) * cache[i - 1] - ((seq_t)70) * cache[i - 2];
    }
    cache_len = (size_t)n + 1;   /* all values up to n are now cached */

    return cache[n];
}
