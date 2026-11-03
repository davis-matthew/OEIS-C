/*
 * A359479  a(1) = 1; a(n) = Sum_{k=2..n} (-1)^k * k * a(floor(n/k)).
 * https://oeis.org/A359479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; a(n) = Σ_{k=2..n} (-1)^k * k * a(⌊n/k⌋)
 * Range:     i8 n <= 27, i16 n <= 447, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Validated: standard, pass, 74 terms through n = 7804 (2026-11-03)
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
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_max = 0;

    /* Ensure we have room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed */
    if (n <= cache_max)
        return cache[n];

    /* Compute all missing entries up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 2; k <= i; ++k) {
            idx_t m = i / k;
            seq_t prod = (seq_t)k * cache[m];
            if (k & 1)
                sum -= prod;   /* sign = -1 for odd k */
            else
                sum += prod;   /* sign = +1 for even k */
        }
        cache[i] = sum;
    }
    cache_max = n;
    return cache[n];
}
