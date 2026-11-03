/*
 * A287051  a(0) = 0, a(1) = 1; a(2*n) = gpf(a(n)), a(2*n+1) = a(n) + a(n+1),
 *          where gpf(a(n)) is the greatest prime dividing a(n) for a(n) >= 2
 *          and 1 if a(n) = 1 (A006530).
 * https://oeis.org/A287051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2*n) = gpf(a(n)), a(2*n+1) = a(n) + a(n+1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 101 terms through n = 100 (2026-11-03)
 * Helpers:   largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can store up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing values sequentially */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if ((i & 1) == 0)   /* even index */
        {
            uint64_t val64 = (uint64_t)cache[i >> 1];
            uint64_t gp = largest_prime_factor(val64);
            cache[i] = (seq_t)gp;
        } else                       /* odd index */
        {
            cache[i] = cache[i >> 1] + cache[(i >> 1) + 1];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
