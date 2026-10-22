/*
 * A328422  Number of paths from 2 to n via maps of the form x -> x + x^j,
 *          where j is a nonnegative integer.
 * https://oeis.org/A328422
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of paths from 2 to n using maps x -> x + x^j (j ≥
 *            0)
 * Range:     u8 n <= 39, u16 n <= 169, u32 n <= 1181, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the number of paths from 2 to n.
 * For n < 2 the function returns 0 (undefined by OEIS). */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Allocate DP array: dp[y] holds number of ways to reach y.  */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return 0;                   /* allocation failure – not expected */

    dp[2] = 1;                      /* start point */

    for (idx_t y = 2; y <= n; ++y)
    {
        seq_t cur = dp[y];
        if (!cur)
            continue;

        /* j = 0: x -> x + 1 */
        if (y + 1 <= n)
            dp[y + 1] += cur;

        /* j >= 1: x -> x + x^j */
        seq_t power = (seq_t)y;            /* y^1 */
        while (1)
        {
            seq_t target = (seq_t)y + power;
            if (target > (seq_t)n)
                break;

            idx_t t = (idx_t)target;      /* target fits in idx_t (<= n) */
            dp[t] += cur;

            /* Stop before next multiplication would exceed n or overflow. */
            if (power > (seq_t)n / y)
                break;
            power *= y;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
