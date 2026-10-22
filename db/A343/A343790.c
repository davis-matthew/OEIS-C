/*
 * A343790  Number of ordered partitions of an n-set without blocks of size 7.
 * https://oeis.org/A343790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n, k≠7} binom(n,k) * a(n-k), a(0)=1 (Ordered
 *            Bell numbers with blocks of size 7 forbidden)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* precomputed values of a(n) */
static size_t cache_cap = 0;     /* current allocated capacity */
static size_t cache_len = 0;     /* number of values already computed (cache_len>=1 when cache[0] is set) */

seq_t a(idx_t n)
{
    /* Ensure enough space for n+1 elements */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        while (new_cap < n + 1) new_cap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base case if not yet done */
    if (cache_len == 0) {
        cache[0] = 1;      /* a(0) = 1 */
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t cur = (idx_t)cache_len; cur <= n; ++cur) {
        seq_t sum = 0;
        seq_t bin = 1;    /* binomial( cur, 0 ) */
        for (idx_t k = 1; k <= cur; ++k) {
            /* Update binomial(cur, k) iteratively */
            bin = bin * (seq_t)(cur - k + 1) / (seq_t)k;
            if (k == 7) continue;              /* skip block size 7 */
            sum += bin * cache[cur - k];
        }
        cache[cur] = sum;
    }
    cache_len = (size_t)(n + 1);

    return cache[n];
}
