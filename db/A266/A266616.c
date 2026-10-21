/*
 * A266616  Number of OFF (white) cells in the n-th iteration of the "Rule 41"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-4)-a(n-6) for n > 5 Initial values: a(0)=0,
 *            a(1)=3, a(2)=3, a(3)=4, a(4)=8, a(5)=3 Computes using memoised
 *            recurrence (O(n) overall)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: contains computed a[0..computed_last] values.
       Cap is the allocated array size. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t computed_last = (size_t)(-1);   /* sentinel: nothing computed yet */

    /* Ensure sufficient capacity. */
    if ((size_t)(n) + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)(n) + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* If the requested index is already computed, just return it. */
    if (computed_last != (size_t)(-1) && n <= (idx_t)computed_last)
        return cache[n];

    /* Compute values from the first missing index up to n. */
    idx_t start = (computed_last == (size_t)(-1)) ? 0
                   : (idx_t)(computed_last + 1);
    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)0;
        else if (i == 1)     val = (seq_t)3;
        else if (i == 2)     val = (seq_t)3;
        else if (i == 3)     val = (seq_t)4;
        else if (i == 4)     val = (seq_t)8;
        else if (i == 5)     val = (seq_t)3;
        else                 /* i >= 6 */
            val = cache[i-2] + cache[i-4] - cache[i-6];
        cache[i] = val;
    }
    computed_last = (size_t)n;

    return cache[n];
}
