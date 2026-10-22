/*
 * A344419  a(n) = n*a(n-1) + n^(n mod 2), a(0) = 0.
 * https://oeis.org/A344419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n^(n mod 2), a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc */

seq_t a(idx_t n)
{
    /* persistent cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory: fallback */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* extend cache to cover n if needed */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory: fallback */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prod = (seq_t)i * cache[i - 1];
            seq_t add  = (i & 1) ? (seq_t)i : (seq_t)1;
            cache[i] = prod + add;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
