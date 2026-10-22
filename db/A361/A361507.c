/*
 * A361507  a(0) = 1; thereafter a(n) = floor((9/4)*a(n-1)) + 1.
 * https://oeis.org/A361507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0: a(n) = floor((9/4)*a(n-1)) + 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Extend the cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                    /* allocation failure, value undefined */
        cache = tmp;

        size_t start = cache_size;
        if (start == 0) {                 /* initialize a(0) */
            cache[0] = (seq_t)1;
            start = 1;
        }
        for (size_t i = start; i < new_cap; ++i) {
            seq_t prev = cache[i - 1];
            /* a(i) = floor((9/4)*prev) + 1
               = 2*prev + floor(prev/4) + 1, avoiding overflow */
            cache[i] = ((seq_t)2 * prev + (prev >> 2) + (seq_t)1);
        }
        cache_size = new_cap;
    }
    return cache[(size_t)n];
}
