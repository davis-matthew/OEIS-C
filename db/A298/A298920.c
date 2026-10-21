/*
 * A298920  Number of n X 4 0..1 arrays with every element equal to 1, 3, 5, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-6)-a(n-7)-a(n-8) for n>8 * with initial
 *            values: * a(1)=1, a(2)=8, a(3)=2, a(4)=3, a(5)=7, * a(6)=6,
 *            a(7)=9, a(8)=20
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n=1..8 */
    static const seq_t base[9] = {0,          /* unused 0 */
                                 1,          /* a(1) */
                                 8,          /* a(2) */
                                 2,          /* a(3) */
                                 3,          /* a(4) */
                                 7,          /* a(5) */
                                 6,          /* a(6) */
                                 9,          /* a(7) */
                                 20 };       /* a(8) */

    /* Dynamic cache of computed values. */
    static seq_t *cache = NULL;   /* cache[0] unused, cache[1..] hold a(n) */
    static idx_t  last   = 0;     /* highest n that has been computed */

    /* If n is within the base range, return immediate value. */
    if (n <= 8)
        return base[(size_t)n];

    /* Initialise cache on first call. */
    if (cache == NULL) {
        /* Allocate space for indices 0..8 (index 0 unused). */
        cache = (seq_t *)calloc(9, sizeof(seq_t));
        /* Fill base values. */
        for (size_t i = 1; i <= 8; ++i)
            cache[i] = base[i];
        last = 8;
    }

    /* If n is already computed, return it. */
    if (n <= last)
        return cache[(size_t)n];

    /* Grow the cache to accommodate index n. */
    size_t new_size = (size_t)n + 1;   /* indices 0..n inclusive */
    if (new_size > (size_t)last + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* allocation failure; sequence not defined */
        cache = tmp;
    }

    /* Compute missing values from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* All indices used in recurrence are now guaranteed to be >=1. */
        cache[(size_t)i] = cache[(size_t)(i-1)]    +
                           cache[(size_t)(i-2)]    +
                           cache[(size_t)(i-6)]    -
                           cache[(size_t)(i-7)]    -
                           cache[(size_t)(i-8)];
    }
    last = n;

    return cache[(size_t)n];
}
