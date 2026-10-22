/*
 * A376112  a(0) = 1; a(n) = (1/2) * Sum_{k=1..n} (3^k-1) * a(k-1) * a(n-k).
 * https://oeis.org/A376112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/2) * Σ_{k=1..n} (3^k - 1) * a(k-1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache      */
    static idx_t cache_len = 0;    /* number of terms computed so far */

    /* Grow the cache if the requested index is not yet stored */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 64;          /* overshoot a little */
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base case a(0) = 1 */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute all terms up to n that are not yet known */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        seq_t pow3 = 3;               /* 3^1 for the first k=1 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod = cache[k-1] * cache[i-k];   /* a(k-1) * a(i-k) */
            seq_t term = prod * pow3 - prod;          /* prod * (3^k - 1) */
            sum += term;
            pow3 *= 3;               /* advance 3^(k+1) for next iteration */
        }
        cache[i] = sum / 2;          /* divide by 2 as per the recurrence */
    }

    cache_len = n + 1;
    return cache[n];
}
