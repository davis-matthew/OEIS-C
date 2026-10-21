/*
 * A267912  Number of 1 X n 0..2 arrays with every repeated value in every row
 *          unequal to the previous repeated value, and in every column equal
 *          to the previous repeated value, and new values introduced in
 *          row-major sequential order.
 * https://oeis.org/A267912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 8*a(n-3) for n>5 with a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=11, a(5)=29
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Compute OEIS sequence A267912. */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence is defined for n ≥ 1 only */
        return 0;

    /* Static cache of previously computed terms.
       cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* largest index that has been computed */

    /* Base values for indices 1 … 5. */
    static const seq_t base[6] = {0, 1, 2, 4, 11, 29};

    /* If we need a longer cache, extend it and compute any new terms. */
    if (n > cached_len) {
        size_t new_len = (size_t)n + 1;              /* indices 0…n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)                                    /* realloc failed */
            return 0;
        cache = tmp;

        /* Compute new terms, starting at cached_len+1. */
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            size_t ui = (size_t)i;                   /* unsigned index for array access */

            if (i <= 5) {                           /* base cases */
                cache[ui] = base[i];
            } else {                                 /* recurrence for i ≥ 6 */
                cache[ui] = (3 * cache[ui - 1] +
                              2 * cache[ui - 2]) -
                             8 * cache[ui - 3];
            }
        }
        cached_len = n;                               /* new maximum computed */
    }

    return cache[(size_t)n];
}
