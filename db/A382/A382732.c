/*
 * A382732  Number of proper hypercubic bipartitions of n.
 * https://oeis.org/A382732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 0, a(n) = a(2^ceil(log2(n)) - n) + 1 for n > 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9844 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple iterative computation with lazy caching.  */

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
    static seq_t *cache = NULL;     /* array of cached values  */
    static idx_t cache_size = 0;    /* current allocated size  */

    /* Make sure the cache can hold values up to 'n'.  */
    ensure_cache(n, &cache, &cache_size);

    /* Initialise base cases once.  */
    if (n >= 0) cache[0] = 0;
    if (n >= 1) cache[1] = 0;

    /* Compute values iteratively from 2 up to n.  */
    idx_t pow2 = 1; /* Current power of two (smallest >= current i) */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Advance pow2 until it is at least i. */
        while (pow2 < i)
            pow2 <<= 1;

        /* The recurrence index j is always < i for i>1. */
        idx_t j = pow2 - i;
        cache[i] = cache[j] + 1;
    }

    return cache[n];
}
