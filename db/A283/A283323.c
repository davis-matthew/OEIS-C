/*
 * A283323  a(n) = 4*a(n-2)+1 with initial terms 1,3,7.
 * https://oeis.org/A283323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2)+1, a(0)=1, a(1)=3, a(2)=7
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 792 (2026-11-03)
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
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of already computed terms   */
    static size_t cache_cap = 0;   /* current capacity of cache          */

    /* initialise cache (first call) */
    if (cache == NULL) {
        /* at least 3 terms are needed initially */
        ensure_cache(3, &cache, &cache_cap);
        cache_len = 3;
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 7;
    }

    /* return early if already computed */
    if (n < cache_len)
        return cache[n];

    /* make sure cache can hold up to n */
    if (cache_cap < (size_t)(n + 1))
        ensure_cache(n + 1, &cache, &cache_cap);

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 3) { /* already initialized */
            continue;
        }
        seq_t val = cache[i - 2] * 4 + 1;
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
