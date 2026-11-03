/*
 * A259131  Numbers n such that 13*n^2 + 52 is a square.
 * https://oeis.org/A259131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - a(n-2); a(1)=3, a(2)=36
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
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

static seq_t *a_cache     = NULL;
static size_t a_cache_cap = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at 1 */

    /* Initialise the first two terms once */
    if (max_computed == 0) {
        ensure_cache(2, &a_cache, &a_cache_cap);
        a_cache[1] = 3;
        a_cache[2] = 36;
        max_computed = 2;
    }

    /* Compute any missing terms up to n */
    if (n > max_computed) {
        ensure_cache(n, &a_cache, &a_cache_cap);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            a_cache[i] = 11 * a_cache[i-1] - a_cache[i-2];
        }
        max_computed = n;
    }

    return a_cache[n];
}
