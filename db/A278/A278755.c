/*
 * A278755  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 65", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A278755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-2) - 14*a(n-4) + 8*a(n-6) for n > 5 with initial
 *            values a(0)=1, a(1)=0, a(2)=3, a(3)=8, a(4)=7, a(5)=48
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* cache size in slots (max index + 1) */

    /* Ensure cache is initialized to the first six terms */
    if (cap == 0) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)48;
        cap = 6;
    }

    /* If the requested index is already in the cache, return it */
    if ((size_t)n < cap)
        return cache[(size_t)n];

    /* Grow the cache to hold all indices up to n */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        /* Compute all new terms from cap up to n inclusive */
        for (size_t i = cap; i < new_cap; ++i) {
            /* recurrence uses only the first six terms already filled */
            cache[i] = 7 * cache[i - 2] - 14 * cache[i - 4] + 8 * cache[i - 6];
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
