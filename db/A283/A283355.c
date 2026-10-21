/*
 * A283355  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 621", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 100*a(n-2) - 100*a(n-3)   for n >= 8
 * Initial values (n = 0 .. 7):
 *   a(0) = 1
 *   a(1) = 10
 *   a(2) = 100
 *   a(3) = 1011
 *   a(4) = 10001
 *   a(5) = 101110
 *   a(6) = 1000110
 *   a(7) = 10111110
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Local constants for the first eight terms. */
    static const seq_t init_vals[8] = {
        (seq_t)1,
        (seq_t)10,
        (seq_t)100,
        (seq_t)1011,
        (seq_t)10001,
        (seq_t)101110,
        (seq_t)1000110,
        (seq_t)10111110
    };

    /* Cached values for successive calls. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements presently in cache */

    /* Return a known initial value immediately. */
    if (n < 8)
        return init_vals[n];

    /* Initialise cache on the first call. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(8 * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        cache_size = 8;
    }

    /* Extend cache up to the required index n. */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failed */
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)100 * cache[i-2]
                     - (seq_t)100 * cache[i-3];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
