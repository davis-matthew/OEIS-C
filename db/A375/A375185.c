/*
 * A375185  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 1, 2, 3, or 5.
 * https://oeis.org/A375185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) + a(n-6) for n>=6; a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=5, a(5)=7
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t known_max = 0;
    static char initialized = 0;
    static const seq_t init_vals[6] = {1, 2, 3, 4, 5, 7};

    /* initialise cache for first 6 values */
    if (!initialized) {
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(*cache));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        known_max = 5;
        initialized = 1;
    }

    /* if already computed, return directly */
    if (n <= known_max)
        return cache[n];

    /* increase capacity if necessary */
    if (cap <= n) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = known_max + 1; i <= n; ++i) {
        if (i < 6)
            cache[i] = init_vals[i];
        else
            cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 5] + cache[i - 6];
    }
    known_max = n;

    return cache[n];
}
