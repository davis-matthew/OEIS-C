/*
 * A280222  a(n) = (-3)^(n-1)*a(n-1) + a(n-2).
 * https://oeis.org/A280222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-3)^(n-1)*a(n-1) + a(n-2) a(0) = 0, a(1) = 1 Recurrence
 *            is evaluated iteratively with memoisation
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 65 (2026-11-03)
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
    /* Memoisation cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;      /* current allocated size of cache */
    static idx_t computed_max = 0;   /* last index that has been computed */
    static seq_t pow_minus3 = 0;     /* (-3)^(k-1) for the current k */

    /* Initialise cache and base values on first call. */
    if (cache == NULL) {
        /* ensure space for indices 0 and 1 */
        ensure_cache(1, &cache, &capacity);

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed_max = 1;
        pow_minus3   = (seq_t)(-3);   /* exponent (k-1) = 1 for k = 2 */
    }

    /* If the requested index is already known, return it. */
    if (n <= computed_max)
        return cache[n];

    /* Grow the cache so that it can hold at least (n+1) elements. */
    ensure_cache(n, &cache, &capacity);

    /* Compute and store all subsequent terms up to n. */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t ak = pow_minus3 * cache[k - 1] + cache[k - 2];
        cache[k] = ak;
        pow_minus3 = pow_minus3 * (seq_t)(-3);   /* advance power for next k */
    }

    computed_max = n;
    return cache[n];
}
