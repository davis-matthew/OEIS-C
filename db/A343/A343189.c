/*
 * A343189  a(1) = ... = a(4) = 1; a(n+4) = Sum_{d|n} mu(n/d) * a(d).
 * https://oeis.org/A343189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343189 :
    a(1) = a(2) = a(3) = a(4) = 1;
    a(n) = Σ_{d | (n-4)} μ((n-4)/d) * a(d)   for n ≥ 5    */
/*  Here μ is the Möbius function (available as mu_of in utils).   */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mu_of.h"   /* provides int mu_of(idx_t) */

/* existing type definitions:
   - idx_t : unsigned 64‑bit index type
   - seq_t : signed integer type (int8_t … __int128 or arbitrary precision) */

static seq_t *cache = NULL;      /* cached sequence values: cache[n] = a(n) */
static idx_t  cap   = 0;         /* current cache capacity (indices 0..cap-1) */
static idx_t  last  = 0;         /* highest index already computed   */

/* Ensure the cache can hold up to index n (inclusive). */
static void ensure_cache(idx_t n) {
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        /* New slots are uninitialized; we will set them before use. */
        cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n = 0 or negative, not required */

    /* initialise the first four terms once */
    if (last == 0) {
        ensure_cache(4);
        cache[1] = cache[2] = cache[3] = cache[4] = (seq_t)1;
        last = 4;
    }

    /* If the requested term is among the cached ones, return it. */
    if (n <= last) return cache[n];

    /* Compute forward from last+1 up to n. */
    idx_t start = (last < 4) ? 5 : last + 1;  /* compute only terms ≥5 */
    start = (start < 5) ? 5 : start;
    for (idx_t k = start; k <= n; ++k) {
        idx_t m = k - 4;      /* m = n-4 for current k */
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                /* divisor d */
                int mu1 = mu_of(m / d);
                sum += (seq_t)mu1 * cache[d];

                idx_t other = m / d;
                if (other != d) {
                    /* divisor other = m/d */
                    int mu2 = mu_of(d);
                    sum += (seq_t)mu2 * cache[other];
                }
            }
        }
        ensure_cache(k);
        cache[k] = sum;
    }
    last = n;
    return cache[n];
}
