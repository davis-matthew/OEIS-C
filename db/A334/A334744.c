/*
 * A334744  a(1) = 1; a(n) = -Sum_{d|n, d < n} bigomega(n/d) * a(d), where
 *          bigomega = A001222.
 * https://oeis.org/A334744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d < n} bigomega(n/d) * a(d) a(1) = 1 (OEIS
 *            A334744)
 * Range:     i8 n <= 8639, i16 n <= 9503, i32 n <= 9503, i64 n <= 9503,
 *            i128 n <= 9503, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"

static seq_t *cache = NULL;      /* Cached sequence values, 1‑based   */
static size_t cache_cap = 0;     /* Current allocated size (index 0 unused) */
static idx_t  max_computed = 0;  /* Largest index computed so far */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)(n + 1);   /* +1 because index 0 is unused */
    if (cache_cap < needed) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory: terminate */
            abort();
        }
        cache = new_cache;
        cache_cap = needed;
    }
}

/* Return a(n) for n >= 1 (returns 0 for n == 0). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n==0, guard against UB */
    if (n <= max_computed) return cache[n];

    /* Expand cache to hold up to n */
    ensure_cache_size(n);

    /* Compute values from max_computed+1 up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        if (m == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t d1 = d;
                if (d1 < m) {
                    seq_t w = bigomega((idx_t)(m / d1));
                    sum += w * cache[d1];
                }
                idx_t d2 = m / d;
                if (d2 != d1 && d2 < m) {
                    seq_t w = bigomega((idx_t)(m / d2));
                    sum += w * cache[d2];
                }
            }
        }
        cache[m] = -sum;
    }
    max_computed = n;
    return cache[n];
}
