/*
 * A362357  Bisection of Chebyshev {S(n, 5)}_{n>=0}; the even part.
 * https://oeis.org/A362357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 23*a(n-1) - a(n-2)
   Base values: a(0) = 1, a(1) = 24   (derived from a(-1) = -1, a(-2) = -24) */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static idx_t  cache_size = 0;    /* number of values cached (effectively cache_size-1 is max n cached) */

seq_t a(idx_t n) {
    /* Special cases for the first two terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return (seq_t)0;   /* Out‑of‑memory guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
    }

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;          /* we need indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return (seq_t)0;   /* Out‑of‑memory guard */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)23 * cache[i - 1] - cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
