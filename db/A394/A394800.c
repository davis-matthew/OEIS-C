/*
 * A394800  a(0) = a(1) = a(2) = a(3) = 1; a(n) = a(n-1) + (n-3)*a(n-4).
 * https://oeis.org/A394800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for 0 ≤ n ≤ 3 a(n) = a(n‑1) + (n‑3) * a(n‑4) for n ≥ 4
 *            (OEIS A394800)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 30, u64 n <= 52, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internally memoise computed values, guaranteeing correct results
   irrespective of call order.  The memoisation list grows
   lazily as larger indices are requested. */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;  /* number of entries currently stored */

/* Return the nth term of A394800. */
seq_t a(idx_t n)
{
    /* The first four terms are always 1. */
    if (n <= 3)
        return (seq_t)1;

    /* Initialise the cache if this is the first request > 3. */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        /* cache[0] .. cache[3] are already zero; set them to 1. */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* Extend the cache to cover up to n, if necessary. */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – fall back */
            /* In a production environment we might handle this error
               differently, but for the exercise we simply return 0. */
            return 0;
        }
        cache = tmp;
        /* Compute new terms sequentially. */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* (n-3) can be safely promoted to seq_t because i is small. */
            seq_t mul = (seq_t)(i - 3) * cache[i - 4];
            cache[i] = cache[i - 1] + mul;
        }
        cache_size = new_size;
    }

    return cache[n];
}
