/*
 * A257932  Expansion of 1/(1-x-x^2-x^3+x^5+x^7).
 * https://oeis.org/A257932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-5) - a(n-7) initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=12,
 *            a(6)=22, a(7)=38
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 3903 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* Cache of computed terms */
    static size_t cap   = 0;      /* Allocated capacity of cache */
    static idx_t last   = 7;      /* Highest index already computed */

    /* Initialise cache with the first 8 known terms, if not yet done */
    if (!cache) {
        /* Allocate space for indices 0..7 at least */
        ensure_cache(7, &cache, &cap);
        static const seq_t init_vals[8] = {1, 1, 2, 4, 7, 12, 22, 38};
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        last = 7;
    }

    /* If the requested term has already been computed, return it */
    if (n <= last)
        return cache[n];

    /* Grow the cache if necessary to include index n */
    ensure_cache(n, &cache, &cap);

    /* Compute new terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = cache[i-1] + cache[i-2] + cache[i-3]
                    - cache[i-5] - cache[i-7];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
