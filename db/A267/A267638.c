/*
 * A267638  Number of n X 2 0..1 arrays with every repeated value in every row
 *          and column unequal to the previous repeated value, and new values
 *          introduced in row-major sequential order.
 * https://oeis.org/A267638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - 8*a(n-4) + 8*a(n-5);
 *            Initial values: a(1)=2, a(2)=8, a(3)=18, a(4)=50, a(5)=98
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;

    if (n < 1)
        return 0;

    /* initialise cache with first 5 terms if not yet done */
    if (!cache) {
        cache = (seq_t *)calloc(6, sizeof(seq_t));
        if (!cache)
            return 0;                  /* allocation failure – unlikely */
        cache[1] = 2;
        cache[2] = 8;
        cache[3] = 18;
        cache[4] = 50;
        cache[5] = 98;
        cache_n = 5;
    }

    /* extend cache up to n */
    if (cache_n < n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                  /* allocation failure – unlikely */
        cache = tmp;

        for (idx_t i = cache_n + 1; i <= n; ++i) {
            seq_t v1 = cache[i-1];
            v1 += 6 * cache[i-2];
            v1 += 8 * cache[i-5];

            seq_t v2 = 6 * cache[i-3];
            v2 += 8 * cache[i-4];

            cache[i] = v1 - v2;
        }
        cache_n = n;
    }

    return cache[n];
}
