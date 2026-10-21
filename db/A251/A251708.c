/*
 * A251708  6-step Fibonacci sequence starting with (0,0,1,0,0,0).
 * https://oeis.org/A251708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+6) = a(n) + a(n+1) + a(n+2) + a(n+3) + a(n+4) + a(n+5) a(0)
 *            = 0, a(1) = 0, a(2) = 1, a(3) = 0, a(4) = 0, a(5) = 0
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Initial 6 terms of the sequence */
static const seq_t init_vals[6] = {0, 0, 1, 0, 0, 0};

/* Computes the 6‑step Fibonacci term a(n). */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed terms   */
    static size_t cache_size = 0;   /* number of elements stored */
    static size_t cache_cap  = 0;   /* currently allocated size   */

    /* Allocate and initialise cache on first call */
    if (cache == NULL) {
        /* Ensure at least 6 entries are allocated */
        cache_cap = (n >= 6 ? (size_t)n + 1 : 6);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        cache_size = 6;
    }

    /* If the requested term is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3]
                  + cache[i-4] + cache[i-5] + cache[i-6];
        cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
