/*
 * A375424  Expansion of e.g.f. exp(-x * (1 - x)) / (1 - x)^2.
 * https://oeis.org/A375424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1)+3*(n-1)*a(n-2)-2*(n-1)*(n-2)*a(n-3) Initial
 *            values: a(0)=1, a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a cached array that grows as needed.
   The helper ensure_cache is included from the utils library. */

#include <stddef.h>                     /* size_t */
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
    static seq_t *cache = NULL;         /* cached values */
    static size_t cap   = 0;            /* current allocation size */
    static idx_t  max_n = (idx_t)-1;    /* highest index computed so far */

    /* Make sure we have room for indices 0..2 at least */
    if (cache == NULL)
        ensure_cache(2, &cache, &cap);

    /* First time: set base values */
    if (max_n == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)1;
        if (cap > 2) cache[2] = (seq_t)5;

        /* If the requested index is in the base range, return it */
        if (n <= 2) {
            max_n = n;
            return cache[n];
        }
        max_n = 2;                                 /* we have computed up to 2 */
    }

    /* If we already know the requested value, return it */
    if (n <= max_n)
        return cache[n];

    /* Grow the cache if necessary */
    if (n > (idx_t)cap - 1)
        ensure_cache(n, &cache, &cap);

    /* Compute new terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t term1 = ((seq_t)i) * cache[i - 1];
        seq_t term2 = ((seq_t)(i - 1)) * (seq_t)3 * cache[i - 2];
        seq_t term3 = ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * (seq_t)2 * cache[i - 3];
        cache[i] = term1 + term2 - term3;
    }

    max_n = n;
    return cache[n];
}
