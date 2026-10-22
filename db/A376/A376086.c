/*
 * A376086  a(0) = 1; a(n) = Sum_{k=0..n-1} (3*k+2) * a(k) * a(n-k-1).
 * https://oeis.org/A376086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (3*k+2) * a(k) * a(n-1-k) a(0) = 1
 *            (initial value)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The following function is required by the build environment.
   It returns the nth term of OEIS sequence A376086.
   The terms are computed in increasing order and memoised
   for subsequent calls with arbitrary call order. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms.
       cache[i] contains a(i) for 0 <= i < cache_len.
       The cache is grown as needed. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored terms */

    /* If this is the first call, allocate space for required terms. */
    if (cache_len == 0) {
        if (n == 0) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache)              /* allocation failure – abort */
                return (seq_t)0;
            cache[0] = (seq_t)1;
            cache_len = 1;
            return cache[0];
        } else {
            /* Allocate for 0..n */
            cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
            if (!cache)
                return (seq_t)0;
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
    }

    /* If requested term already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Extend cache up to n (inclusive). */
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 (caller should treat as error). */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = tmp;
    }

    /* Compute new terms from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = ((seq_t)3 * (seq_t)k) + (seq_t)2;
            sum += term * cache[k] * cache[i - 1 - k];
        }
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
