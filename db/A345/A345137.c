/*
 * A345137  a(1) = a(2) = 1; a(n+2) = Sum_{d|n, d < n} a(d).
 * https://oeis.org/A345137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1; a(n)=Σ_{d|n-2, d<(n-2)} a(d) for n≥3
 * Range:     u8 n <= 925, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;   /* cached values, indexed by n   */
static size_t cap = 0;        /* current allocated size        */
static idx_t  computed = 0;   /* largest n that has been computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n) {
    if ((size_t)n < cap) return;
    ensure_cache(n, &cache, &cap);
}

seq_t a(idx_t n) {
    /* Allocate enough space for index n. */
    ensure_capacity(n);

    /* Initialise the base values if they are not yet set. */
    if (computed < 1) { cache[1] = (seq_t)1; computed = 1; }
    if (computed < 2) { cache[2] = (seq_t)1; computed = 2; }

    /* If the value is already computed, return it directly. */
    if (n <= computed) return cache[n];

    /* Compute all values up to n. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k <= 2) { cache[k] = (seq_t)1; continue; }   /* safety */

        idx_t m = k - 2;          /* proper divisors of m */
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;
                if (d < m)         sum += cache[d];
                if (e < m && e != d) sum += cache[e];
            }
        }
        cache[k] = sum;
    }
    computed = n;
    return cache[n];
}
