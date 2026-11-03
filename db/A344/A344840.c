/*
 * A344840  a(0) = 1; a(n) = 5 * Sum_{k=1..n} binomial(n,k) * a(k-1).
 * https://oeis.org/A344840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 5 * Σ_{k=1..n} binomial(n,k) * a(k-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
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
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed = 0;   /* number of values already stored (next to compute) */

    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* make sure the cache can hold up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise the first element if this is the first call */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* compute all missing entries up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;   /* binomial(i,0) */

        /* k runs from 1 to i; b‌تواند вычислять binomial(i,k) iteratively */
        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient: C(i,k) = C(i,k-1) * (i-k+1) / k */
            binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
            sum += binom * cache[k - 1];
        }

        cache[i] = 5 * sum;
    }

    computed = n + 1;
    return cache[n];
}
