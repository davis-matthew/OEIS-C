/*
 * A292488  p-INVERT of the odd positive integers, where p(S) = 1 - S - 5 S^2.
 * https://oeis.org/A292488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) + 13*a(n-3) + 5*a(n-4) for n >= 5;
 *            initial values: a(0)=1, a(1)=9, a(2)=52, a(3)=261, a(4)=1323
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initial values for the first 5 terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)9, (seq_t)52,
                                   (seq_t)261, (seq_t)1323 };

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* if requested index is beyond current cache, extend it */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)5 * cache[i-1]
                     - (seq_t)2 * cache[i-2]
                     + (seq_t)13 * cache[i-3]
                     + (seq_t)5 * cache[i-4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
