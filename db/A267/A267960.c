/*
 * A267960  Number of n X 1 0..2 arrays with every repeated value in every row
 *          and column greater than the previous repeated value.
 * https://oeis.org/A267960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) - 8*a(n-3) + 24*a(n-4) - 16*a(n-6),
 *            with a(1)=3, a(2)=9, a(3)=24, a(4)=63, a(5)=159, a(6)=394
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1: a(0) is unused */
    if (n == 0) return 0;

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    /* Ensure capacity for indices up to n and at least up to 6 */
    idx_t need = n + 1;
    if (need < 7) need = 7;
    if (need > cache_size) {
        idx_t new_size = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure */
        /* initialize new slots to zero */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Ensure base values are set */
    if (cache[1] == 0) cache[1] = 3;
    if (cache[2] == 0) cache[2] = 9;
    if (cache[3] == 0) cache[3] = 24;
    if (cache[4] == 0) cache[4] = 63;
    if (cache[5] == 0) cache[5] = 159;
    if (cache[6] == 0) cache[6] = 394;

    /* If already computed, return the value */
    if (cache[n] != 0) return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* may already be computed */
        seq_t left  = 6 * cache[i-1] + 24 * cache[i-4];
        seq_t right = 9 * cache[i-2] + 8 * cache[i-3] + 16 * cache[i-6];
        cache[i] = left - right;
    }

    return cache[n];
}
