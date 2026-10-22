/*
 * A363813  Number of permutations of [n] that avoid the patterns 2-41-3,
 *          3-14-2, 2-1-4-3, and 4-5-3-1-2.
 * https://oeis.org/A363813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 37*a(n-2) + 62*a(n-3) - 47*a(n-4)
 *                 + 16*a(n-5) - 2*a(n-6)   for n ≥ 6;
 *         a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=78 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial six terms */
    static const seq_t init[6] = {1, 1, 2, 6, 21, 78};

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Needed cache size */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        /* Compute newly added terms */
        for (size_t i = cache_cap; i < needed; ++i) {
            if (i < 6) {
                cache[i] = init[i];
            } else {
                /* Positive part */
                seq_t pos = cache[i-1] * 10
                           + cache[i-3] * 62
                           + cache[i-5] * 16;
                /* Negative part */
                seq_t neg = cache[i-2] * 37
                           + cache[i-4] * 47
                           + cache[i-6] * 2;
                /* Result */
                cache[i] = pos - neg;
            }
        }
        cache_cap = needed;
    }
    return cache[n];
}
