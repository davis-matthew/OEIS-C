/*
 * A329465  Expansion of Sum_{k>=1} (-1 + Product_{j>=1} 1 / (1 - x^(k*j*(j +
 *          1)/2))).
 * https://oeis.org/A329465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A007294(d) (A007294(n) = #partitions of n into
 *            triangular numbers)
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static arrays to hold the triangular‑partition counts (dp)
   and the divisor sums giving a(n).  They are resized whenever the
   maximum n seen so far grows. */
static seq_t *dp = NULL;
static seq_t *ans = NULL;
static idx_t max_precalc = 0;

/* Recompute dp and ans for all numbers up to N (inclusive). */
static void recompute_upto(idx_t N)
{
    if (N <= max_precalc)                      /* nothing to do */
        return;

    /* Allocate fresh arrays of length N+1 */
#ifdef __cplusplus
    seq_t *new_dp = new seq_t[N + 1];
    seq_t *new_ans = new seq_t[N + 1];
#else
    seq_t *new_dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *new_ans = (seq_t*)malloc((N + 1) * sizeof(seq_t));
#endif

    /* Initialise all entries to zero */
    for (idx_t i = 0; i <= N; ++i) {
        new_dp[i] = 0;
        new_ans[i] = 0;
    }
    new_dp[0] = (seq_t)1;   /* base case for partitions */

    /* Compute dp[n] = number of partitions of n into triangular numbers */
    for (idx_t j = 1; ; ++j) {
        seq_t t_seq = (seq_t)j * (j + 1) / 2;   /* triangular number */
        if (t_seq > N)
            break;
        idx_t t = (idx_t)t_seq;
        for (idx_t s = t; s <= N; ++s)
            new_dp[s] += new_dp[s - t];
    }

    /* Compute ans[n] = sum_{d|n} dp[d]  (divisor sum) */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t val = new_dp[i];
        for (idx_t m = i; m <= N; m += i)
            new_ans[m] += val;
    }

    /* Free previous arrays */
    if (dp) {
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
    }
    if (ans) {
#ifdef __cplusplus
        delete[] ans;
#else
        free(ans);
#endif
    }

    /* Adopt the newly computed arrays */
    dp = new_dp;
    ans = new_ans;
    max_precalc = N;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* behaviour below offset is irrelevant */
        return (seq_t)0;
    if (n > max_precalc)
        recompute_upto(n);
    return ans[n];
}
