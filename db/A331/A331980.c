/*
 * A331980  Number of compositions (ordered partitions) of n into distinct odd
 *          parts > 1.
 * https://oeis.org/A331980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} k! * p(k, (n - k(k+2))/2) * where p(k, m) is
 *            the number of integer partitions of m into at most k parts. *
 *            This follows from writing each distinct odd part as 3+2c with c
 *            distinct, * transforming the counting problem into partitions
 *            with a size restriction. * The algorithm runs...
 * Range:     u8 n <= 37, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Compute the maximum possible number of parts k: k(k+2) <= n */
    idx_t max_k = 0;
    while ((max_k + 1) * ((max_k + 1) + 2) <= n)
        ++max_k;

    /* Maximum value of m = (n - k(k+2))/2 is at most n/2 */
    idx_t max_m = n / 2;
    size_t cols = max_m + 1;          /* number of columns for m */
    size_t rows = max_k + 1;          /* number of rows for k */

    /* Allocate DP table for partitions p(k, m) */
    seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!dp) return 0;                 /* allocation failure: return 0 */

    /* dp[0][0] = 1 */
    dp[0] = 1;

    /* Fill DP table:
       p(k, m) = p(k-1, m) + p(k, m-k)   (if m >= k)
       This counts partitions into at most k parts.
    */
    for (idx_t k = 1; k <= max_k; ++k) {
        size_t k_offset = k * cols;
        size_t km1_offset = (k - 1) * cols;
        for (idx_t m = 0; m <= max_m; ++m) {
            seq_t val = dp[km1_offset + m];   /* partitions without using an additional part */
            if (m >= k)
                val += dp[k_offset + (m - k)]; /* add a part of size k */
            dp[k_offset + m] = val;
        }
    }

    /* Precompute factorials up to max_k */
    seq_t *fact = (seq_t *)malloc((max_k + 1) * sizeof(seq_t));
    if (!fact) { free(dp); return 0; }
    fact[0] = 1;
    for (idx_t i = 1; i <= max_k; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Sum over all possible k */
    seq_t ans = 0;
    for (idx_t k = 0; k <= max_k; ++k) {
        long diff = (long)n - (long)k * ((long)k + 2);
        if (diff < 0 || (diff & 1))
            continue;                     /* n - k(k+2) must be even */
        idx_t m = (idx_t)(diff >> 1);      /* m = (n - k(k+2))/2 */
        if (m > max_m)
            continue;
        seq_t ways = dp[k * cols + m];
        ans += ways * fact[k];
    }

    free(dp);
    free(fact);
    return ans;
}
