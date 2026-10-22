/*
 * A336106  Number of integer partitions of n whose greatest part is at most
 *          one more than the sum of the other parts.
 * https://oeis.org/A336106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n whose greatest part is
 *            at most one more than the sum of the other parts. Equivalently:
 *            a(n) = p(n, floor((n+1)/2)), where p(n,k) counts partitions of n
 *            with largest part ≤ k
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;     /* dp[n] = partition counts with parts ≤ current k   */
static seq_t *ans = NULL;    /* ans[n] = required value up to n                  */
static idx_t  computed_max = (idx_t)-1;   /* maximum n for which ans[] is valid */

/* Recompute all answers up to 'up_to' from scratch. */
static void recompute(idx_t up_to)
{
    size_t N = (size_t)up_to;
    seq_t *new_dp  = (seq_t*)realloc(dp,  (N + 1) * sizeof(seq_t));
    seq_t *new_ans = (seq_t*)realloc(ans, (N + 1) * sizeof(seq_t));
    dp  = new_dp;
    ans = new_ans;

    /* initialize arrays */
    for (size_t i = 0; i <= N; ++i) {
        dp[i]  = (seq_t)0;
        ans[i] = (seq_t)0;
    }
    dp[0]  = (seq_t)1;
    ans[0] = (seq_t)1;

    idx_t maxK = (up_to + 1) / 2;   /* ceil(n/2) for the largest n */

    for (idx_t k = 1; k <= maxK; ++k) {
        /* update partition counts with parts up to k */
        for (idx_t n = k; n <= up_to; ++n) {
            dp[n] = dp[n] + dp[n - k];
        }

        /* fill results for n whose threshold equals current k */
        idx_t n_start = 2 * k - 1;            /* first n with ceil(n/2)==k */
        if (n_start > up_to) break;
        idx_t n_end = 2 * k;                  /* last such n */
        if (n_end > up_to) n_end = up_to;

        for (idx_t n = n_start; n <= n_end; ++n) {
            ans[n] = dp[n];
        }
    }

    computed_max = up_to;
}

/* Ensure that ans[n] has been computed for requested 'n'. */
static void ensure(idx_t n)
{
    if (computed_max < n || computed_max == (idx_t)-1) {
        recompute(n);
    }
}

seq_t a(idx_t n)
{
    ensure(n);
    return ans[n];
}
