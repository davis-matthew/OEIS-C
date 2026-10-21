/*
 * A281737  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 390", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A281737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - c[n] * where c[n] are the coefficients of the
 *            numerator polynomial * of the generating function: * (12*x^25 +
 *            4*x^24 + x^23 + 3*x^22 + 3*x^21 + 3*x^20 + 3*x^19 + * 3*x^18 +
 *            3*x^17 + 3*x^16 + 3*x^15 + 3*x^14 + 3*x^13 + 3*x^12 + * 55*x^11
 *            + 15*x^10 - 2*x^9 + x^7 + 3*x^6 - x^5 - x^...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const long long coeffP[26] = {
    -1, -3, 0, -2, -1, -1, 3, 1, 0, -2,
    15, 55, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 1, 4, 12
};

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        /* allocate or reallocate */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        /* compute new terms from old size to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev2 = (i >= 2) ? cache[i-2] : (seq_t)0;
            seq_t term = prev2 * (seq_t)4;
            long long c = (i < 26) ? coeffP[i] : 0LL;
            if (c >= 0) {
                term = term - (seq_t)c;
            } else {
                term = term + (seq_t)(-c);
            }
            cache[i] = term;
        }
        cache_size = new_size;
    }

    return cache[n];
}
