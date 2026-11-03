/*
 * A378990  Dirichlet inverse of the binary weight of n (A000120).
 * https://oeis.org/A378990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Σ_{d|n, d<n} A000120(n/d) * a(d)
 * Range:     i8 n <= 692, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 9387 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Static cache for already computed values, 1‑indexed. */
static seq_t *cache = NULL;      /* array of values, cache[0] unused */
static size_t cache_cap = 0;     /* current capacity of the array (>= n+1) */
static idx_t  last_computed = 0; /* largest n that has been computed */

/* Ensure the cache can hold values up to n inclusive. */
static void ensure_cache(idx_t n)
{
    /* Need capacity at least n+1 (since we index from 1). */
    if (n + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)n + 128;   /* a small buffer to reduce reallocs */
    if (cache == NULL) {
        cache = mem_new(seq_t, new_cap);
    } else {
        cache = mem_resize(seq_t, cache, new_cap);
        /* zero‑initialise the newly added portion */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n = 0, return 0 */

    if (n > last_computed) {
        ensure_cache(n);
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            if (k == 1) {
                cache[1] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            /* iterate over all divisors d of k, d < k */
            for (idx_t i = 1; i <= k / i; ++i) {
                if (k % i == 0) {
                    idx_t d1 = i;
                    idx_t d2 = k / i;
                    if (d1 < k) {
                        seq_t w = (seq_t)__builtin_popcountll((unsigned long long)(k / d1));
                        sum += w * cache[d1];
                    }
                    if (d2 != d1 && d2 < k) {
                        seq_t w = (seq_t)__builtin_popcountll((unsigned long long)(k / d2));
                        sum += w * cache[d2];
                    }
                }
            }
            cache[k] = -sum;
        }
        last_computed = n;
    }
    return cache[n];
}
