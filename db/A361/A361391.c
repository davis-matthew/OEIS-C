/*
 * A361391  Number of strict integer partitions of n with non-integer mean.
 * https://oeis.org/A361391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..⌊(√(8n+1)-1)/2⌋} [n mod k ≠ 0] · P(n - k(k-1)/2 ,
 *            k), where P(m,k) is the number of ordinary partitions of m into
 *            exactly k parts. a(0) = 1
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the largest k with k(k+1)/2 ≤ n.
   Works for all n that can be represented in idx_t. */
static idx_t max_k_for_n(idx_t n)
{
    idx_t k = 0;
    while (true)
    {
        /* (k+1)*(k+2)/2 ≤ n  ⟺  (k+1)*(k+2) ≤ 2*n   */
        unsigned __int128 left  = (unsigned __int128)(k + 1) * (k + 2);
        unsigned __int128 right = (unsigned __int128)2 * n;
        if (left <= right)
            ++k;
        else
            break;
    }
    return k;
}

/* memoised DP table for partitions P(m,k) */
static seq_t **dp = NULL;          /* dp[m][k] = number of partitions of m into k parts   */
static idx_t  dp_nmax = 0;         /* current maximum m allocated                        */
static idx_t  dp_kmax = 0;         /* current maximum k allocated                        */

static void free_dp(void)
{
    if (!dp) return;
    for (idx_t i = 0; i <= dp_nmax; ++i)
        free(dp[i]);
    free(dp);
    dp = NULL;
    dp_nmax = 0;
    dp_kmax = 0;
}

/* Ensure dp is computed for all m ≤ n. */
static void ensure_dp(idx_t n)
{
    if (n <= dp_nmax)
        return;                           /* already ready */

    free_dp();                             /* discard smaller table    */

    dp_nmax = n;
    dp_kmax = max_k_for_n(dp_nmax);

    dp = (seq_t **)malloc((dp_nmax + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= dp_nmax; ++i)
    {
        dp[i] = (seq_t *)calloc(dp_kmax + 1, sizeof(seq_t)); /* zero‑init */
    }

    /* Base case */
    dp[0][0] = (seq_t)1;                   /* P(0,0)=1, all others are 0 */

    /* Build the table */
    for (idx_t m = 1; m <= dp_nmax; ++m)
    {
        for (idx_t k = 1; k <= dp_kmax; ++k)
        {
            seq_t val = dp[m - 1][k - 1];
            if (m >= k)
                val += dp[m - k][k];
            dp[m][k] = val;
        }
    }
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                     /* a(0) = 1 */

    ensure_dp(n);

    seq_t ans = (seq_t)0;
    idx_t kmax = max_k_for_n(n);
    for (idx_t k = 1; k <= kmax; ++k)
    {
        if (n % k == 0)                       /* mean n/k would be integer */
            continue;
        /* m = n - k(k-1)/2 */
        seq_t m = (seq_t)n - (seq_t)k * ((seq_t)k - 1) / 2;
        if (m < (seq_t)k)                     /* cannot have k parts */
            continue;
        ans += dp[(idx_t)m][k];
    }
    return ans;
}
