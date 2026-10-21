/*
 * A271900  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^17)).
 * https://oeis.org/A271900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5 and 17 *
 *            Computation: dynamic programming for the classic unbounded coin
 *            * change problem. This implementation recomputes the entire
 *            table * for each call; it runs in O(n) time and uses only O(n)
 *            memory, well * within the requirements for n up to ~...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate the dynamic programming table */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)
        return 0;            /* allocation failure – should not happen */

    /* Initialise */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;
    dp[0] = 1;               /* one way to make sum 0 */

    /* Denominations allowed */
    const idx_t parts[3] = {3, 5, 17};

    /* Count partitions */
    for (int p = 0; p < 3; ++p)
    {
        idx_t v = parts[p];
        for (idx_t sum = v; sum <= n; ++sum)
        {
            dp[(size_t)sum] += dp[(size_t)(sum - v)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
