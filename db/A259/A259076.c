/*
 * A259076  Powers of 80.
 * https://oeis.org/A259076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80*a(n-1) (a(0) = 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 465 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    static seq_t *cache = NULL;   /* array of computed powers */
    static size_t cache_cap = 0;  /* capacity of allocated array */
    static idx_t last_n = 0;      /* highest index computed so far */

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* On first call, initialise a(0) */
    if (last_n == 0) {
        cache[0] = (seq_t)1;
        last_n = 0;
    }

    /* Compute new powers up to n if needed */
    if (n > last_n) {
        for (idx_t i = last_n + 1; i <= n; ++i)
            cache[i] = cache[i - 1] * (seq_t)80;
        last_n = n;
    }

    return cache[n];
}
