/*
 * A373566  Expansion of x - 1/(x - 1/(x + 1)).
 * https://oeis.org/A373566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A373566 – Expansion of x - 1/(x - 1/(x + 1))
    Recurrence: a(0)=1, a(1)=3, a(2)=3, a(3)=5,
    a(n) = a(n-1) + a(n-2) for n ≥ 4                                       */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* Static cache that grows as needed. */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    /* If the request is beyond the current cache, extend. */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;

        /* Allocate a new block that can hold all needed entries. */
        seq_t *new_cache = alloc_cache(new_sz);
        if (!new_cache)
            abort();  /* out‑of‑memory */

        /* Copy existing values into the new block (if any). */
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_sz * sizeof(seq_t));
            free_cache(cache);
        }

        cache   = new_cache;
        cache_sz = new_sz;

        /* Initialise the first few entries if they are required. */
        if (cache_sz > 0) cache[0] = (seq_t)1;
        if (cache_sz > 1) cache[1] = (seq_t)3;
        if (cache_sz > 2) cache[2] = (seq_t)3;
        if (cache_sz > 3) cache[3] = (seq_t)5;

        /* Fill in the remaining entries using the recurrence. */
        if (cache_sz > 4) {
            for (idx_t i = 4; i < cache_sz; ++i)
                cache[i] = cache[i-1] + cache[i-2];
        }
    }

    return cache[n];
}
