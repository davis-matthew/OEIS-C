/*
 * A275907  Expansion of (1+x-x^3) / (1-4*x-x^4-x^5).
 * https://oeis.org/A275907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-4) + a(n-5), n > 4 Initial values: a(0) =
 *            1, a(1) = 5, a(2) = 20, a(3) = 79, a(4) = 317
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 950 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;        /* pointer to cached sequence values */
    static idx_t cache_size = 0;       /* number of values currently stored (<= cache_cap) */
    static idx_t cache_cap  = 0;       /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with capacity for 5 initial values */
        cache_cap = 5;
        cache = mem_new(seq_t, (size_t)cache_cap);
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)79;
        cache[4] = (seq_t)317;
        cache_size = 5;
    }

    /* if requested index is within stored values, return it */
    if (n < cache_size) return cache[n];

    /* extend cache capacity if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        do {
            new_cap *= 2;
        } while (n >= new_cap);
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_cap);
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 4 * cache[i-1] + cache[i-4] + cache[i-5];
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}
