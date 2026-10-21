/*
 * A295045  Number of n X 2 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0 or 2 1s.
 * https://oeis.org/A295045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) - a(n-4) * Empirical
 *            recurrence * Initial values: a(1)=3, a(2)=8, a(3)=19, a(4)=48
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle non-positive indices (undefined in OEIS) */
    if (n < 1) return 0;

    /* static cache for memoization */
    static seq_t *memo = NULL;
    static idx_t   cap   = 0;   /* allocated size (>= n+1) */
    static idx_t   max_n = 0;   /* highest index computed */

    /* allocate/reallocate cache if needed */
    if (cap < n + 1)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!new_mem) return 0; /* allocation failed */
        memo = new_mem;
        cap  = (idx_t)new_cap;
    }

    /* compute needed values up to n */
    idx_t i;
    for (i = max_n + 1; i <= n; ++i)
    {
        if (i == 1)        memo[1] = (seq_t)3;
        else if (i == 2)   memo[2] = (seq_t)8;
        else if (i == 3)   memo[3] = (seq_t)19;
        else if (i == 4)   memo[4] = (seq_t)48;
        else
        {
            /* recurrence: a(n) = 2*a(n-1) + a(n-2) + a(n-3) - a(n-4) */
            seq_t val = (seq_t)2 * memo[i - 1] + memo[i - 2]
                      + memo[i - 3] - memo[i - 4];
            memo[i] = val;
        }
    }
    max_n = (max_n < n) ? n : max_n;   /* update highest computed index */

    return memo[n];
}
