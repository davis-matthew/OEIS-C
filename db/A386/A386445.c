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
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

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
