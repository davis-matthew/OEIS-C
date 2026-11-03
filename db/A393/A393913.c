/*
 * A393913  a(n) = 1 + n^2 + Sum_{k=0..n-1} a(k) * a(n-1-k).
 * https://oeis.org/A393913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n^2 + Sum_{k=0..n-1} a(k) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 990 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
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
    static size_t cap = 0;   /* allocated size */
    static size_t count = 0; /* number of values already computed */

    /* Ensure space for indices 0..n */
    ensure_cache(n + 1, &cache, &cap);

    /* Initialise base case if needed */
    if (count == 0) {
        cache[0] = (seq_t)1;
        count = 1;
    }

    /* Compute values up to n */
    for (idx_t i = count; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* convolution part: sum_{k=0}^{i-1} a(k)*a(i-1-k) */
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }
        seq_t ii = (seq_t)i;
        cache[i] = (seq_t)1 + ii * ii + sum;
    }
    count = (size_t)(n + 1);
    return cache[n];
}
