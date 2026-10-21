/*
 * A265992  Number of 2 X n integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors not equal to
 *          itself.
 * https://oeis.org/A265992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) + a(n-4) + 2*a(n-5) + 2*a(n-6) + 3*a(n-7) +
 *            2*a(n-8) - 4*a(n-9) - 4*a(n-10) - 4*a(n-11) for n>14 Initial
 *            values: a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=4, a(6)=6, a(7)=7,
 *            a(8)=11, a(9)=15, a(10)=26, a(11)=48, a(12)=82, a(13)=135,
 *            a(14)=227
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 45, u64 n <= 87, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static const seq_t init_vals[14] = {
        1, 1, 2, 2, 4, 6, 7, 11, 15, 26, 48, 82, 135, 227
    };

    if (n == 0) return 0;          /* offset 1; n=0 is unused */

    /* initialize cache with first 14 terms */
    if (cache_size == 0) {
        cache_cap = 14;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        for (size_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
        cache_size = 14;
    }

    /* if requested term already memoized, return it */
    if (n <= cache_size) return cache[n - 1];

    /* ensure enough capacity for up to n */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        size_t idx0 = (size_t)(i - 1);   /* index in cache for a(i) */

        seq_t sum_pos = 3 * cache[idx0 - 3] + cache[idx0 - 4]
                       + 2 * cache[idx0 - 5] + 2 * cache[idx0 - 6]
                       + 3 * cache[idx0 - 7] + 2 * cache[idx0 - 8];

        seq_t sum_neg = 4 * cache[idx0 - 9] + 4 * cache[idx0 -10]
                       + 4 * cache[idx0 -11];

        cache[idx0] = sum_pos - sum_neg;
    }

    cache_size = n;
    return cache[n - 1];
}
