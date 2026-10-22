/*
 * A385830  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k^2) * a(k) * a(n-1-k).
 * https://oeis.org/A385830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (1 + k^2) * a(k) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Computes sequence A385830: a(0)=1, a(n)=∑_{k=0}^{n-1}(1+k^2)a(k)a(n-1-k)   */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity                   */
    static size_t cache_len  = 0;   /* number of terms actually stored       */

    /* Resize cache if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = seq_realloc(cache, cache_cap, needed);
        cache     = tmp;
        cache_cap = needed;
    }

    /* Ensure base case */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            seq_t kseq = (seq_t)k;                      /* cast k to seq_t */
            seq_t term = cache[k] * cache[i - 1 - k];   /* a(k)*a(i-1-k)   */
            term *= (1 + kseq * kseq);                  /* *(1+k^2)        */
            sum += term;
        }
        cache[i] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
