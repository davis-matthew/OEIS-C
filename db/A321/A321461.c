/*
 * A321461  a(n) = -a(n-1) + 2*a(n-2) + a(n-3), a(0) = -1, a(1) = -2, a(2) =
 *          -4.
 * https://oeis.org/A321461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + a(n-3), a(0) = -1, a(1) = -2, a(2) =
 *            -4
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 37, i64 n <= 75, i128 n <= 150,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static idx_t cache_cap  = 0;
    static bool  initialized = false;
    static idx_t computed_up_to = 2;   /* we know the first three terms */

    /* Handle the base cases immediately */
    if (n == 0) return (seq_t)(-1);
    if (n == 1) return (seq_t)(-2);
    if (n == 2) return (seq_t)(-4);

    /* Make sure the cache can hold up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the first three entries once */
    if (!initialized) {
        cache[0] = (seq_t)(-1);
        cache[1] = (seq_t)(-2);
        cache[2] = (seq_t)(-4);
        initialized = true;
    }

    /* Compute any missing terms up to n */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = -cache[i-1] + (seq_t)2 * cache[i-2] + cache[i-3];
        }
        computed_up_to = n;
    }

    return cache[n];
}
