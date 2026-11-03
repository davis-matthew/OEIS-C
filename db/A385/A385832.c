/*
 * A385832  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k^4) * a(k) * a(n-1-k).
 * https://oeis.org/A385832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence for OEIS A385832:
 *   a(0) = 1
 *   a(n) = Σ_{k=0}^{n-1} (1 + k^4) * a(k) * a(n-1-k)   for n ≥ 1
 */
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
    /* static cache and its state */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t next_computed = 0;   /* first index that still needs to be computed */

    /* make sure the cache has room for indices 0..n */
    if (cache == NULL)
        ensure_cache(n + 1, &cache, &cache_cap);
    else if ((idx_t)cache_cap <= n)
        ensure_cache(n + 1, &cache, &cache_cap);

    /* initialise a(0) if it has not been done yet */
    if (next_computed == 0) {
        cache[0] = (seq_t)1;
        next_computed = 1;
    }

    /* compute any missing values up to n */
    for (idx_t i = next_computed; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t kseq = (seq_t)k;
            seq_t k2   = kseq * kseq;      /* k^2          */
            seq_t k4   = k2 * k2;          /* k^4          */
            seq_t w    = k4 + (seq_t)1;    /* 1 + k^4      */

            seq_t prod = w * cache[k];
            prod *= cache[i - 1 - k];
            sum += prod;
        }
        cache[i] = sum;
    }
    next_computed = n + 1;
    return cache[n];
}
