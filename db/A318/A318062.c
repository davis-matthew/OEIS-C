/*
 * A318062  Number of nX2 0..1 arrays with every element unequal to 1, 2, 3 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4); Initial values:
 *            a(1)=1, a(2)=5, a(3)=18, a(4)=67. Recurrence holds for all n ≥ 5
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t max_n = 0;   /* largest n already computed */

    size_t N = (size_t)n;     /* index to compute */

    /* Allocate cache on first call */
    if (cache == NULL)
    {
        cache = (seq_t*)malloc((N + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;   /* fallback, should not happen */
        max_n = 0;
    }

    /* Extend cache if necessary */
    if (N > max_n)
    {
        /* Resize array to hold indices up to N */
        cache = (seq_t*)realloc(cache, (N + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;   /* fallback */

        /* Compute all new terms */
        for (size_t i = max_n + 1; i <= N; ++i)
        {
            if (i == 1)          cache[i] = 1;
            else if (i == 2)      cache[i] = 5;
            else if (i == 3)      cache[i] = 18;
            else if (i == 4)      cache[i] = 67;
            else                  cache[i] = 3*cache[i-1] + 3*cache[i-2]
                                   - cache[i-3] - cache[i-4];
        }
        max_n = N;
    }

    return cache[N];
}
