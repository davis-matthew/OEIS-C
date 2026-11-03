/*
 * A321461  a(n) = -a(n-1) + 2*a(n-2) + a(n-3), a(0) = -1, a(1) = -2, a(2) =
 *          -4.
 * https://oeis.org/A321461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + a(n-3), a(0) = -1, a(1) = -2, a(2) =
 *            -4
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 37, i64 n <= 75, i128 n <= 150,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 952 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    static seq_t *cache     = NULL;
    static idx_t cache_cap  = 0;
    static bool  initialized = false;
    static idx_t computed_up_to = 2;   /* we know the first three terms */

    /* Handle the base cases immediately */
    if (n == 0) return (seq_t)(-1);
    if (n == 1) return (seq_t)(-2);
    if (n == 2) return (seq_t)(-4);

    /* Make sure the cache can hold up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the first three entries once */
    if (!initialized) {
        cache[0] = (seq_t)(-1);
        cache[1] = (seq_t)(-2);
        cache[2] = (seq_t)(-4);
        initialized = true;
    }

    /* Compute any missing terms up to n */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = -cache[i-1] + (seq_t)2 * cache[i-2] + cache[i-3];
        }
        computed_up_to = n;
    }

    return cache[n];
}
