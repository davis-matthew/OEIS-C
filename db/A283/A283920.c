/*
 * A283920  a(n) = (1 + Sum_{j=1..K-2} a(n-j)*a(n-j-1))/a(n-K) with
 *          a(1),...,a(K)=1, where K=8.
 * https://oeis.org/A283920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + Sum_{j=1..K-2} a(n-j)*a(n-j-1)) / a(n-K),
 *            a(1)=...=a(K)=1, K = 8 (off‑shelf)
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 14, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* idx_t and seq_t are typedefs provided by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, but handle 0 safely */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;          /* number of allocated slots (>= n+1) */
    static idx_t   max_cached = 0;         /* last index computed */

    const idx_t K = 8;                     /* recurrence width */
    const idx_t INIT = 8;                  /* initial all‑ones block */

    /* Allocate or grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                        /* allocation failed */
            return (seq_t)0;
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Initialise the first 8 entries to 1 on every call (idempotent) */
    for (idx_t i = 1; i <= INIT && i <= n; ++i)
        cache[i] = (seq_t)1;

    /* Compute any missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= INIT) {                   /* already set to 1 */
            continue;
        }
        /* Sum over j = 1 .. K-2 (i.e. 1 .. 6) */
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= K - 2; ++j) {
            seq_t prod = cache[i - j] * cache[i - j - 1];
            sum += prod;
        }
        seq_t numerator   = sum + (seq_t)1;
        seq_t denominator = cache[i - K];
        cache[i] = numerator / denominator;    /* exact division by definition */
    }

    max_cached = max_cached > n ? max_cached : n;
    return cache[n];
}
