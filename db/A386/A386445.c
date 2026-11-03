/*
 * A386445  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} k^4 * a(k) * a(n-1-k).
 * https://oeis.org/A386445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} k^4 * a(k) * a(n-1-k)
 *            (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
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

/* Sequence entry type defined elsewhere (seq_t, idx_t).  */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;     /* number of entries already computed */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Ensure the cache is large enough for indices 0..n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base case if not already done */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;

        for (idx_t k = 0; k < i; ++k) {
            seq_t kseq = (seq_t)k;
            seq_t kpow4 = kseq * kseq;      /* k^2  */
            kpow4 = kpow4 * kpow4;          /* k^4  */

            seq_t term = kpow4 * cache[k];
            term = term * cache[i - 1 - k];
            sum = sum + term;
        }

        seq_t value = cache[i - 1] + sum;
        cache[i] = value;
    }

    cache_len = n + 1;
    return cache[n];
}
