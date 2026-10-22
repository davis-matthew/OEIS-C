/*
 * A341398  Number of integer solutions to (x_1)^2 + (x_2)^2 + ... + (x_9)^2
 *          <= n.
 * https://oeis.org/A341398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=0}^{n} r9(m) where r9(m) is the number of integer
 *            9‑tuples (x1,…,x9) with x1²+…+x9² = m, counting signs and order
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root (floor) for an unsigned 64‑bit integer. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* allocate arrays for DP */
    seq_t *dp   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !next)
    {
        free(dp);
        free(next);
        return (seq_t)0;
    }

    /* weight for each square: 1 for zero, 2 for non‑zero (±i) */
    idx_t maxRoot = isqrt_u64(n);
    seq_t *weight = (seq_t *)calloc((size_t)(maxRoot + 1), sizeof(seq_t));
    if (!weight)
    {
        free(dp);
        free(next);
        return (seq_t)0;
    }

    weight[0] = (seq_t)1;
    for (idx_t i = 1; i <= maxRoot; ++i)
        weight[i] = (seq_t)2;

    /* start with zero variables: one way to have sum 0 */
    dp[0] = (seq_t)1;

    /* perform 9 convolutions */
    for (idx_t var = 0; var < 9; ++var)
    {
        /* clear next array */
        for (idx_t i = 0; i <= n; ++i)
            next[i] = (seq_t)0;

        for (idx_t sum = 0; sum <= n; ++sum)
        {
            seq_t cur = dp[sum];
            if (cur == (seq_t)0)
                continue;
            for (idx_t r = 0; r <= maxRoot; ++r)
            {
                idx_t sq = r * r;
                if (sq > n - sum)
                    break;
                next[sum + sq] += cur * weight[r];
            }
        }

        /* swap dp and next */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* compute prefix sum for a(n) */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i)
        result += dp[i];

    free(dp);
    free(next);
    free(weight);
    return result;
}
