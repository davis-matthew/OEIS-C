/*
 * A360885  G.f. satisfies A(x) = 1 + x * A(x * (1 + x^2)).
 * https://oeis.org/A360885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..⌊(n-1)/3⌋} binomial(n-1-2*k, k) *
 *            a(n-1-2*k)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 23, u64 n <= 39, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 915 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)(-1);

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* Initialise cache if not already */
    if (cache == NULL) {
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = 1;
        last_computed = 0;
    }

    /* Return cached value if already computed */
    if (n <= last_computed)
        return cache[n];

    /* Grow cache to accommodate n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t rem = i - 1 - 2 * k;
            seq_t bin = binomial(rem, k);
            seq_t prod = seq_mul(bin, cache[rem]);  /* bin * a(rem) */
            sum += prod;
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
