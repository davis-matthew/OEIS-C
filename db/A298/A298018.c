/*
 * A298018  Partial sums of A298015.
 * https://oeis.org/A298018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   * a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7) for n >
 *            10 * a(0) = 1, a(1) = 4, a(2) = 10, a(3) = 25, a(4) = 46, * a(5)
 *            = 64, a(6) = 97, a(7) = 145, a(8) = 175, a(9) = 226, a(10) = 298
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..10 */
    static const seq_t init[11] = {
        1, 4, 10, 25, 46, 64, 97, 145, 175, 226, 298
    };

    /* Cache of computed terms */
    static seq_t *cache  = NULL;   /* pointer to array of seq_t */
    static idx_t cache_size = 0;   /* current size of the cache */

    /* Handle the trivial cases directly */
    if (n < 11) {
        return init[n];
    }

    /* Initialise cache on first use */
    if (cache == NULL) {
        /* Allocate space for the initial 11 terms */
        cache_size = 11;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i) {
            cache[i] = init[i];
        }
    }

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed; abort by returning a sentinel value */
            return 0;
        }
        cache = tmp;
        cache_size = new_size;

        /* Compute new terms using the recurrence */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val += cache[i - 3];
            val += cache[i - 3];
            val -= cache[i - 4];
            val -= cache[i - 4];
            val -= cache[i - 6];
            val += cache[i - 7];
            cache[i] = val;
        }
    }

    return cache[n];
}
