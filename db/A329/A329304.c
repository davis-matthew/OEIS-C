/*
 * A329304  Numerators of convergents to A309930, the constant whose continued
 *          fraction representation consists of the cubes, [0; 1, 8, 27, 64,
 *          ...].
 * https://oeis.org/A329304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^3*a(n-1)+a(n-2) a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;   /* number of terms stored (next free index) */

    /* Ensure we have enough space in the cache */
    if (cache == NULL) {
        /* Allocate at least n+1 entries for current request */
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Out of memory – in this kata we simply abort */
            abort();
        }
    } else if ((size_t)(n + 1) > cache_cap) {
        /* Grow buffer geometrically */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already done */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
        } else if (i == 1) {
            cache[1] = 1;
        } else {
            /* Compute n^3 * a(n-1) + a(n-2) using seq_t arithmetic */
            seq_t icube = (seq_t)i * (seq_t)i * (seq_t)i;
            cache[i] = icube * cache[i - 1] + cache[i - 2];
        }
    }
    cache_size = n + 1;   /* we have stored up to index n */

    return cache[n];
}
