/*
 * A350177  a(n) is the number of nonnegative integers that can be represented
 *          by lighting only n segments on a 9-segment display, used by the
 *          Russian postal service.
 * https://oeis.org/A350177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*a(n-3) + 3*a(n-4) + 2*a(n-5) + a(n-6) + a(n-7) for n >
 *            13 with initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=3,
 *            a(5)=2, a(6)=5, a(7)=13, a(8)=17, a(9)=22, a(10)=47, a(11)=86,
 *            a(12)=127, a(13)=211. The function below computes a(n) for any n
 *            ≥ 0, memoizing values
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 14 initial terms, matching the OEIS offset 0. */
    static const seq_t init[14] = {
        0, 0, 0, 2, 3, 2, 5, 13, 17, 22, 47, 86, 127, 211
    };

    /* Static cache for already computed values.  */
    static seq_t *cache      = NULL;   /* pointer to the array */
    static size_t cache_size = 0;      /* allocated size      */
    static size_t last_filled = 0;     /* greatest index computed */

    /* Handle small n directly using the constant table. */
    if (n < 14)
        return init[(size_t)n];

    /* First allocation: store the 14 known values. */
    if (cache == NULL) {
        cache_size   = 14;
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 14; ++i)
            cache[i] = init[i];
        last_filled = 13;
    }

    /* Grow the cache if required.  */
    size_t required = (size_t)n;
    if (cache_size <= required) {
        size_t new_size = cache_size * 2;
        if (new_size <= required)
            new_size = required + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache      = tmp;
        cache_size = new_size;
    }

    /* Compute any missing values using the recurrence.  */
    if (last_filled < required) {
        for (size_t i = last_filled + 1; i <= required; ++i) {
            cache[i] = (seq_t)2 * cache[i - 3]
                     + (seq_t)3 * cache[i - 4]
                     + (seq_t)2 * cache[i - 5]
                     +      cache[i - 6]
                     +      cache[i - 7];
        }
        last_filled = required;
    }

    return cache[required];
}
