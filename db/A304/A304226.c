/*
 * A304226  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2 or
 *          7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed via the empirical recurrence a(1)=8, a(2)=5,
 *            a(3)=6, a(4)=11, a(5)=20, a(6)=46, a(7)=97, a(8)=189, a(n) =
 *            a(n-1) + 5*a(n-3) - 4*a(n-6) for n > 8
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function required by the build */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize the cache with the first 8 terms if it hasn't been done yet */
    if (cache_size == 0) {
        cache_size = 9; /* indices 0..8, we ignore index 0 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return 0 as a safe value */

        cache[0] = (seq_t)0;          /* not used */
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)20;
        cache[6] = (seq_t)46;
        cache[7] = (seq_t)97;
        cache[8] = (seq_t)189;
    }

    /* Grow the cache if the requested index is beyond what we have computed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – safe value */

        /* Compute any missing entries sequentially */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* For i <= 8 we already have the values, but the loop never reaches i <= 8
               because cache_size is at least 9. */
            cache[i] = cache[i - 1] + 5 * cache[i - 3] - 4 * cache[i - 6];
        }
        cache_size = new_size;
    }

    return cache[n];
}
