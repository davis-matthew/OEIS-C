/*
 * A385981  a(0) = 1; a(n) = Sum_{k=0..n-1} (k+1) * binomial(k+4,4) *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A385981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385981:
   a(0) = 1;
   for n > 0:
   a(n) = Sum_{k=0..n-1} (k+1) * C(k+4,4) * C(n-1,k) * a(k) * a(n-1-k)   */
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

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;
    static bool initialized = false;

    if (!initialized) {
        /* ensure space for a[0] */
        ensure_cache(1, &cache, &cache_cap);
        cache[0] = 1;              /* a(0) = 1 */
        max_computed = 0;
        initialized = true;
    }

    /* grow cache if needed */
    ensure_cache(n + 1, &cache, &cache_cap);

    if (max_computed < n) {
        for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
            seq_t sum = 0;
            seq_t binom_nk = 1;          /* C(cur-1,0) */
            for (idx_t k = 0; k < cur; ++k) {
                /* (k+1) * C(k+4,4) */
                seq_t binom_k4 = ((seq_t)(k + 4) * (k + 3) * (k + 2) * (k + 1)) / 24;
                seq_t coeff   = (seq_t)(k + 1) * binom_k4;

                seq_t term = coeff * binom_nk * cache[k] * cache[cur - 1 - k];
                sum += term;

                /* update binom_nk for next k:
                   C(cur-1, k+1) = C(cur-1, k) * (cur-1-k)/(k+1) */
                if (k + 1 < cur)
                    binom_nk = binom_nk * ((seq_t)(cur - 1 - k)) / ((seq_t)(k + 1));
            }
            cache[cur] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
