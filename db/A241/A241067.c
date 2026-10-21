/*
 * A241067  Number of partitions p of n into distinct parts such that max(p) =
 *          -1 + 2*min(p).
 * https://oeis.org/A241067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n==1)?1: sum_{m=2}^{⌊(n+1)/3⌋} counts of subsets of
 *            {m+1,…,2m–2} summing to n–3m+1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 0) return 0;

    /* DP array of size n+1 (maximum possible T). */
    size_t dp_len = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(dp_len, sizeof(seq_t));
    if (!dp) return 0;  /* unlikely, but safe */

    seq_t result = 0;
    idx_t maxm = (n + 1) / 3;

    for (idx_t m = 2; m <= maxm; ++m)
    {
        idx_t base = 3 * m - 1;
        if (base > n) continue;
        idx_t T = n - base;           /* required sum of interior parts */

        /* reset dp[0..T] */
        memset(dp, 0, (size_t)(T + 1) * sizeof(seq_t));
        dp[0] = 1;

        if (m >= 3)                    /* interior numbers exist */
        {
            idx_t start = m + 1;
            idx_t end   = 2 * m - 2;
            for (idx_t w = start; w <= end; ++w)
            {
                if (w > T) break;
                size_t wi = (size_t)w;
                for (idx_t t = T; t >= wi; --t)
                    dp[t] += dp[t - wi];
            }
        }
        result += dp[T];
    }

    free(dp);
    return result;
}
