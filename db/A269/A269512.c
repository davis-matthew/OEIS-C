/*
 * A269512  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 342", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)-a(n-6)-a(n-7) (n>6)
 *            with initial values a(0)=4, a(1)=3, a(2)=16, a(3)=-7, a(4)=36,
 *            a(5)=-21, a(6)=64
 * Range:     i8 n <= 9, i16 n <= 127, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

static const seq_t init_vals[7] = {4, 3, 16, -7, 36, -21, 64};

seq_t a(idx_t n)
{
    /* initialise cache if needed */
    if (!cache) {
        size_t init_size = 7;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely in tests */
        for (idx_t i = 0; i < 7; ++i) {
            cache[i] = init_vals[i];
        }
        cache_size = init_size;
    }

    /* extend cache up to n if required */
    if ((size_t)(n) + 1 > cache_size) {
        size_t new_size = (size_t)(n) + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i <= 6) {
                val = init_vals[i];
            } else {
                val = -cache[i-1] + cache[i-2] + cache[i-3] +
                      cache[i-4] + cache[i-5] - cache[i-6] - cache[i-7];
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
