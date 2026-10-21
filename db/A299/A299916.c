/*
 * A299916  a(n) = A299914(2n+1).
 * https://oeis.org/A299916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 12*a(n-2); a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy, thread‑unsafe memoisation.  Works for all call orders. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise base values on first call. */
    if (cache_len == 0) {
        cache_len = 2;                                   /* we need a(0) and a(1) */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;                     /* out‑of‑memory: graceful */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
    }

    /* Return cached result if already computed. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache to include a(n). */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) return (seq_t)0;                           /* out‑of‑memory: graceful */
    cache     = tmp;
    cache_len = (idx_t)new_len;

    /* Compute any missing terms via the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        cache[i] = 9 * cache[i-1] - 12 * cache[i-2];
    }

    return cache[n];
}
