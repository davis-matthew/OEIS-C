/*
 * A302279  Number of 2 X n 0..1 arrays with every element equal to 1, 2 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 4*a(n-4) for n > 5; initial terms:
 *            a(1)=0, a(2)=3, a(3)=7, a(4)=10, a(5)=27
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;

    /* initialise first five terms once */
    if (cached_max < 5) {
        cached_max = 5;
        cache = (seq_t *)realloc(cache, (cached_max + 1) * sizeof(seq_t));
        if (!cache) return 0;
        cache[1] = 0;
        cache[2] = 3;
        cache[3] = 7;
        cache[4] = 10;
        cache[5] = 27;
    }

    /* grow cache if a larger index is requested */
    if (n > cached_max) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + (seq_t)3 * cache[i-2] - (seq_t)4 * cache[i-4];
        }
        cached_max = n;
    }

    return cache[n];
}
