/*
 * A351972  a(n) = 1 + Sum_{k=0..floor((n-1)/2)} a(k) * a(n-2*k-1).
 * https://oeis.org/A351972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351972
 *
 * Formula: a(n) = 1 + Σ_{k=0..⌊(n-1)/2⌋} a(k) * a(n-2k-1)
 * Offset: 0
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
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
#include <stddef.h>
#include <stdbool.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static idx_t   last_computed = 0;
static bool     initialized = false;

seq_t a(idx_t n)
{
    /* initialise cache to hold n+1 entries */
    if (!initialized) {
        ensure_cache(n, &cache, &cache_cap);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last_computed = 0;
        initialized = true;
    } else if (n >= cache_cap) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t maxk = (i == 0) ? 0 : (i - 1) / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            sum += cache[k] * cache[i - 2 * k - 1];
        }
        cache[i] = (seq_t)1 + sum;
    }
    last_computed = n;

    return cache[n];
}
