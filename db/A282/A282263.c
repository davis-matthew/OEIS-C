/*
 * A282263  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 446", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 256*a(n-8) - 256*a(n-10) for n > 17. Initial
 *            values: a(0)=1, a(1)=3, a(2)=6, a(3)=15, a(4)=24, a(5)=56,
 *            a(6)=106, a(7)=255, a(8)=386, a(9)=899, a(10)=1706, a(11)=4095,
 *            a(12)=6162, a(13)=14419, a(14)=27250, a(15)=65107, a(16)=98930,
 *            a(17)=230003
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* cache covers indices 0 .. cache_len-1 */

/* Stored initial values to avoid recomputing them repeatedly. */
static const seq_t init_values[18] = {
    1, 3, 6, 15, 24, 56, 106, 255, 386, 899,
    1706, 4095, 6162, 14419, 27250, 65107, 98930, 230003
};

seq_t a(idx_t n) {
    /* If cache is not large enough, grow it and compute new entries. */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* Compute values from the old size up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 18) {
                cache[i] = init_values[(size_t)i];
            } else {
                cache[i] = cache[i - 2] + ((seq_t)256 * cache[i - 8])
                                 - ((seq_t)256 * cache[i - 10]);
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
