/*
 * A282829  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 521", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-8) - 4*a(n-10) (n >= 18) Initial values (0
 *            <= n <= 17):
 *            1,0,4,0,28,56,84,0,380,248,1364,0,5884,3448,22740,13696,91004,50936
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 18 terms are hard‑coded */
    static const seq_t init[18] = {
        (seq_t)1,      (seq_t)0,  (seq_t)4,      (seq_t)0,
        (seq_t)28,     (seq_t)56, (seq_t)84,     (seq_t)0,
        (seq_t)380,    (seq_t)248,(seq_t)1364,   (seq_t)0,
        (seq_t)5884,   (seq_t)3448,(seq_t)22740,  (seq_t)13696,
        (seq_t)91004,  (seq_t)50936
    };

    if (n < 18)
        return init[(size_t)n];

    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* initialise cache with the 18 known terms */
    if (cache == NULL) {
        cap = 18;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)   /* allocation failure – caller should handle */
            return (seq_t)0;
        for (size_t i = 0; i < 18; ++i)
            cache[i] = init[i];
    }

    /* grow the cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failure – caller should handle */
            return (seq_t)0;
        cache = tmp;

        /* compute terms from the current size up to n */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            seq_t term = (seq_t)4 * cache[(size_t)(i - 2)]
                       + cache[(size_t)(i - 8)]
                       - (seq_t)4 * cache[(size_t)(i - 10)];
            cache[(size_t)i] = term;
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
