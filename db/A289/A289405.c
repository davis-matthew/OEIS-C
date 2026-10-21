/*
 * A289405  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 566", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=110; a(n)=10*a(n-1)+10*a(n-2)-100*a(n-3)
 *            for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* storage of previously computed terms */
    static idx_t cached = 0;        /* number of terms cached (next index to compute) */

    /* Expand cache if needed and compute missing terms */
    if (n >= cached) {
        size_t new_cap = (size_t)(n + 1);                     /* array size needed */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure: return 0 */
        cache = tmp;

        for (idx_t i = cached; i <= n; ++i) {
            size_t ui = (size_t)i;
            if (i == 0)          cache[ui] = 1;
            else if (i == 1)     cache[ui] = 10;
            else if (i == 2)     cache[ui] = 110;
            else                 cache[ui] = 10 * cache[ui - 1]
                                      + 10 * cache[ui - 2]
                                      - 100 * cache[ui - 3];
        }
        cached = n + 1;
    }
    return cache[(size_t)n];
}
