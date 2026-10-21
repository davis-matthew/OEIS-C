/*
 * A286810  Number of non-attacking bishop positions on a cylindrical 2 X 2*n
 *          chessboard.
 * https://oeis.org/A286810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) for n > 3
   Initial values: a(0)=1, a(1)=9, a(2)=49, a(3)=324. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to memoize values.  cache_len is the number of
       elements currently stored (indices 0..cache_len-1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize the cache with the first four terms. */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory: give 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)324;
    }

    /* If the requested index is beyond the cached range, extend the
       cache and compute the missing terms. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory: give 0 */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)8 * cache[i-1] - (seq_t)8 * cache[i-2] + cache[i-3];
        }
        cache_len = new_len;
    }

    return cache[n];
}
