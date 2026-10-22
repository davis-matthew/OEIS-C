/*
 * A377315  a(n) = coefficient of 2^(1/3) in the expansion of (1 + 2^(1/3) +
 *          2^(2/3))^n.
 * https://oeis.org/A377315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1)+3*a(n-2)+a(n-3) with a(0)=0, a(1)=1, a(2)=4 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of valid entries in cache */

    /* if cache not created or insufficient, grow it */
    if (cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* compute new entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)
                cache[i] = (seq_t)0;
            else if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)4;
            else
                cache[i] = (seq_t)3 * cache[i-1] + (seq_t)3 * cache[i-2] + cache[i-3];
        }
        cache_size = (idx_t)new_size;
    }

    return cache[n];
}
