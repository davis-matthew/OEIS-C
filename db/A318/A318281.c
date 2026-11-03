/*
 * A318281  a(n+1) = a(n-a(n)) if a(n-1) != a(n), otherwise a(n+1) = a(n) + 3;
 *          a(1) = a(2) = a(3) = a(4) = 1.
 * https://oeis.org/A318281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-a(n)) if a(n-1) != a(n), otherwise a(n) + 3; Initial
 *            values: a(1) = a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 83 terms through n = 9868 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>   /* for bool support if needed */
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
    /* Static cache for memoization */
    static seq_t *cache      = NULL;
    static idx_t  max_calc   = 0;   /* highest index already computed */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Allocate enough space for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute values up to n if not already done */
    for (idx_t i = max_calc + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = (seq_t)1;
        } else {
            seq_t prev       = cache[i - 1];   /* a(n)   */
            seq_t prevprev   = cache[i - 2];   /* a(n-1) */

            if (prevprev != prev) {
                /* a(n+1) = a(n-a(n))  where n = i-1 */
                idx_t offset = (idx_t)prev;           /* a(n) as index diff */
                idx_t target = (i - 1) - offset;      /* index for a(n-a(n)) */
                /* target is guaranteed to be >= 1 by definition */
                cache[i] = cache[target];
            } else {
                cache[i] = prev + (seq_t)3;
            }
        }
    }
    max_calc = n;
    return cache[n];
}
