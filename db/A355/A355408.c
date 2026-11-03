/*
 * A355408  Expansion of e.g.f. 1/(1 + exp(x) - exp(3*x)).
 * https://oeis.org/A355408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{k=1..n} (3^k-1)·C(n,k)·a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
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
    static size_t cache_computed = 0; /* number of entries already stored */

    /* Make sure the array can hold indices 0..n. */
    ensure_cache((idx_t)(n + 1), &cache, &cache_cap);

    if (cache_computed <= (size_t)n) {
        for (idx_t i = cache_computed; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t binom = 1;          /* C(i,0) */
            seq_t pow3  = 3;          /* 3^1 for k = 1 */
            const seq_t one = 1;

            for (idx_t k = 1; k <= i; ++k) {
                /* Compute C(i,k) iteratively. */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

                seq_t term = binom * cache[i - k];
                seq_t factor = pow3 - one;  /* 3^k - 1 */
                term = term * factor;
                sum = sum + term;

                /* Advance to next power of three. */
                pow3 = pow3 * 3;
            }
            cache[i] = sum;
        }
        cache_computed = (size_t)n + 1;
    }

    return cache[n];
}
