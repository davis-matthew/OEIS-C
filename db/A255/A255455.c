/*
 * A255455  A255454(2^n-1).
 * https://oeis.org/A255455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 20*a(n-3) + 51*a(n-4) - 56*a(n-5) +
 *            46*a(n-6) - 20*a(n-7) + 8*a(n-8) Initial values: a(0)=1, a(1)=5,
 *            a(2)=19, a(3)=73, a(4)=263, a(5)=973, a(6)=3531, a(7)=12965
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_filled = 0;/* number of computed entries */

    /* First eight values of the sequence */
    static const seq_t init_val[8] = {
        1, 5, 19, 73, 263, 973, 3531, 12965
    };

    /* Initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_val[i];
        cache_filled = 8;          /* indices 0..7 are now valid */
    }

    /* If the requested index is already computed, return it */
    if (n < cache_filled)
        return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values from cache_filled up to n (inclusive) */
    for (idx_t i = cache_filled; i <= n; ++i) {
        seq_t val = 6 * cache[i - 1];
        val -= 6 * cache[i - 2];
        val -= 20 * cache[i - 3];
        val += 51 * cache[i - 4];
        val -= 56 * cache[i - 5];
        val += 46 * cache[i - 6];
        val -= 20 * cache[i - 7];
        val += 8 * cache[i - 8];
        cache[i] = val;
    }

    cache_filled = (size_t)n + 1;
    return cache[n];
}
