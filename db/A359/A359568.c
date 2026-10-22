/*
 * A359568  Maximum number of distinct folds after folding a square sheet of
 *          paper n times.
 * https://oeis.org/A359568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) + 4 - n for n > 2, a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute OEIS A359568 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n + 1 > cache_len) {
        /* extend cache to hold indices up to n */
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* initialise base values */
        cache[0] = (seq_t)0;
        if (new_len > 1) cache[1] = (seq_t)1;
        if (new_len > 2) cache[2] = (seq_t)3;

        /* compute new entries */
        idx_t start = cache_len;
        if (start < 3) start = 3;
        for (idx_t i = start; i <= n; ++i) {
            seq_t temp = cache[i - 1] * (seq_t)2 - (seq_t)i;
            cache[i] = temp + (seq_t)4;
        }
        cache_len = new_len;
    }

    return cache[n];
}
