/*
 * A361854  Number of strict integer partitions of n such that (length) *
 *          (maximum) = 2n.
 * https://oeis.org/A361854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts strict integer partitions of n where (length)*(maximum) = 2n */
/* a(n) is 0 for n < 9, as shown in the OEIS entry, but the function
   works for all n >= 1. */

#include <stdint.h>
#include <stdlib.h>

/* Helper to compute floor(sqrt(x)) for unsigned 64‑bit integers.
   Uses 128‑bit arithmetic to avoid overflow. */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = ( __uint128_t )mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* total = 2n, all pairs (k,m) with k*m = 2n and k <= m */
    idx_t total = n * 2;
    seq_t result = 0;

    idx_t max_k = floor_sqrt_u64(total);

    for (idx_t k = 1; k <= max_k; ++k) {
        if (total % k != 0)
            continue;

        idx_t m = total / k;
        if (k > m)               /* ensure that k <= m            */
            continue;
        if (k < 2)                /* length 1 cannot satisfy f.    */
            continue;

        /* need (k-1) distinct parts from 1..(m-1) sum to S = n - m */
        idx_t S = n - m;
        if (S == (idx_t)-1)     /* overflowed? (not possible here)*/
            continue;
        if (S > (idx_t)INT64_MAX) /* keep within bounds                  */
            continue;

        /* Quick feasibility test: min and max sums of k-1 numbers */
        idx_t t = k - 1;
        seq_t minSum = (seq_t)t * (t + 1) / 2;                 /* 1+2+...+t   */
        if (S < minSum)
            continue;

        seq_t maxSum = (seq_t)t * m - (seq_t)t * (t + 1) / 2; /* (m-1)+... */
        if (S > maxSum)
            continue;

        /* DP table: (t+1) rows, (S+1) columns */
        size_t rows = (size_t)(t + 1);
        size_t cols = (size_t)(S + 1);
        seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
        if (!dp)
            return 0;  /* allocation failure: return 0 as safe fallback */

        dp[0] = 1;    /* dp[0][0] = 1 */

        for (idx_t val = 1; val <= m - 1; ++val) {
            /* walk rows backwards to avoid reuse of updated values */
            for (idx_t r = t; r >= 1; --r) {
                seq_t *row_cur = dp + r * cols;
                seq_t *row_prev = dp + (r - 1) * cols;
                for (idx_t s = S; s >= val; --s) {
                    row_cur[s] += row_prev[s - val];
                }
            }
        }

        result += dp[t * cols + S];
        free(dp);
    }

    return result;
}
