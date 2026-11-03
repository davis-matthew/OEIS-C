/*
 * A242234  Number of length n+3+1 0..3 arrays with every value 0..3 appearing
 *          at least once in every consecutive 3+2 elements, and new values
 *          0..3 introduced in order.
 * https://oeis.org/A242234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) Initial values: a(1) =
 *            10, a(2) = 22, a(3) = 43, a(4) = 82
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 64, u128 n <= 132,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 207 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stddef.h>
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

static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;  /* current capacity of cache */
static size_t cache_len = 0;  /* number of computed terms */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)  /* undefined; return 0 as a safeguard */
        return (seq_t)0;

    /* Ensure the cache is large enough for the requested index. */
    ensure_cache(n, &cache, &cache_cap);

    /* If we already have the value, return it immediately. */
    if (n <= (idx_t)cache_len)
        return cache[n-1];

    /* Compute the sequence values up to n (inclusive). */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t val;
        if (i == (idx_t)0)
            val = (seq_t)10;
        else if (i == (idx_t)1)
            val = (seq_t)22;
        else if (i == (idx_t)2)
            val = (seq_t)43;
        else if (i == (idx_t)3)
            val = (seq_t)82;
        else
            val = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];

        cache[i] = val;
    }
    cache_len = n;  /* update length to reflect the new maximum index computed */

    return cache[n-1];
}
