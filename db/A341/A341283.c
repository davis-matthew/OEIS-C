/*
 * A341283  Number of ways to partition n labeled elements into sets of
 *          different sizes of at least 3.
 * https://oeis.org/A341283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=0; a(1)=a(2)=0; for n≥3 a(n) = ∑_{k=3}^{n} C(n,k)
 *            · a(n−k), where each block size is used at most once (distinct
 *            sizes)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 19, u64 n <= 31, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A341283: number of ways to partition
   n labeled elements into blocks of distinct sizes ≥3. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small n are trivial */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;

    /* Work with size_t for array indices */
    size_t N = (size_t)n;

    /* Allocate dynamic array to hold DP values a(0)…a(N) */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failure → zero */

    /* Initialise all entries to 0 except dp[0] = 1 */
    for (size_t i = 0; i <= N; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Iterate over block sizes k = 3 … N.
       dp[n] after processing size k holds the number of partitions of
       n using only block sizes ≤k with all sizes distinct. */
    for (idx_t k = 3; k <= n; ++k)
    {
        /* Pre‑compute the binomial coefficient C(N,k). */
        seq_t comb = (seq_t)1;
        for (idx_t i = 1; i <= k; ++i)
            comb = (comb * (seq_t)(N - i + 1)) / (idx_t)i;

        /* Update dp in reverse order so that dp[n‑k] refers to the
           values computed before the current step (sizes < k). */
        for (size_t ii = N; ii >= (size_t)k; /* size_t is unsigned */
             /* loop body will decrement manually */)
        {
            /* ii is n for this iteration */
            dp[ii] = dp[ii] + comb * dp[ii - (size_t)k];

            /* Last step has ii == k, after it we should exit. */
            if (ii == (size_t)k) break;

            /* Update comb for the next smaller n:
               C(n‑1,k) = C(n,k) * (n‑k) / n .
               Here n = ii, so use (ii - k) and ii. */
            comb = (comb * (seq_t)(ii - (size_t)k)) / (idx_t)ii;

            --ii;   /* explicit decrement to avoid unsigned underflow */
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
