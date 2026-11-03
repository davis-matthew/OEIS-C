/*
 * A343674  a(0) = 1; a(n) = 4 * n * a(n-1) + Sum_{k=0..n-1} binomial(n,k) *
 *          a(k).
 * https://oeis.org/A343674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 4*n*a(n-1) + Sum_{k=0..n-1} binomial(n,k)*a(k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (!initialized) {
        ensure_cache(n, &cache, &cache_cap);
        cache[0] = 1;
        last_computed = 0;
        initialized = true;
    }
    if (n > last_computed) {
        ensure_cache(n, &cache, &cache_cap);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Compute the binomial sum */
            seq_t sum = 0;
            seq_t binom = 1; /* binomial(i,0) */
            for (idx_t k = 0; k < i; ++k) {
                sum += binom * cache[k];
                if (k + 1 < i) {
                    /* Update binom = binom * (i - k) / (k + 1) */
                    binom *= ((seq_t)(i - k));
                    binom /= ((seq_t)(k + 1));
                }
            }
            /* 4 * i * a(i-1) */
            seq_t term = cache[i - 1];
            term *= ((seq_t)i);
            term *= 4;
            cache[i] = term + sum;
        }
        last_computed = n;
    }
    return cache[n];
}
