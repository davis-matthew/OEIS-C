/*
 * A241827  Number of partitions p = [x(1), ..., x(k)], where x(1) >= x(2) >=
 *          ... >= x(k), of n such that min(x(i) - x(i-1)) > number of
 *          distinct parts of p.
 * https://oeis.org/A241827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k≥1, n≥k*(k^2+1)/2} p(n - k*(k^2+1)/2, k) where p(m,
 *            k) is the number of partitions of m into parts not exceeding k.
 *            This follows from the observation that a valid partition has k
 *            distinct parts y_1<...<y_k with y_{i+1}-y_i ≥ k+1. After the
 *            change y_i = z_i + (i-1)(k+1) with z_i≥0 and z...
 * Range:     u8 n <= 52, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C11 / C++17 compliant implementation */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*   seq_t       is the type used for sequence values   (unspecified, could be
    uint64_t, unsigned __int128, or big integer class).        */
seq_t a(idx_t n)
{
    /* Special case: the empty partition gives a(0)=1            */
    if (n == 0) return (seq_t)1;

    /* Cache for computing p(m,k) for all 0≤m≤max_n, 0≤k≤kmax          */
    static seq_t *dp = NULL;          /* flattened 2‑D array       */
    static idx_t  dp_rows = 0;        /* number of rows = max_n+1   */
    static idx_t  dp_cols = 0;        /* number of columns = kmax+1  */

    /* Determine new limits that cover the current query                      */
    idx_t  nmax_needed = n;
    idx_t  kmax_needed = 0;
    for (idx_t k = 1; ; ++k) {
        /* minimal_sum_k = k*(k^2+1)/2  (use 128‑bit to avoid overflow)   */
        unsigned __int128 k128 = k;
        unsigned __int128 min_sum128 = k128 * (k128 * k128 + 1) / 2;
        if (min_sum128 > (unsigned __int128)nmax_needed) break;
        kmax_needed = k;
    }

    /* Recompute the DP table if necessary                                 */
    if (dp_rows < nmax_needed + 1 || dp_cols < kmax_needed + 1) {
        /* free old table if it exists                                     */
        free(dp);

        dp_rows = nmax_needed + 1;
        dp_cols = kmax_needed + 1;
        /* allocate                                       */
        dp = (seq_t *)malloc(dp_rows * dp_cols * sizeof(seq_t));
        if (!dp) { /* allocation failed – return 0 as a safe fallback      */
            return (seq_t)0;
        }

        /* Helper macro to index the flattened array                        */
        #define DP(i,j) dp[((i)*(dp_cols) + (j))]

        /* Base cases: dp[0][*] = 1, dp[*][0] = 0 (except dp[0][0] = 1)   */
        for (idx_t j = 0; j < dp_cols; ++j) DP(0, j) = 1;
        for (idx_t i = 1; i < dp_rows; ++i) DP(i, 0) = 0;

        /* Fill the table using the recurrence:                            */
        /*   p(m,k) = p(m,k-1) + p(m-k,k)   if m >= k      */
        /*   p(m,k) = p(m,k-1)               otherwise      */
        for (idx_t i = 1; i < dp_rows; ++i) {
            for (idx_t j = 1; j < dp_cols; ++j) {
                if (i >= j) {
                    DP(i, j) = DP(i, j-1) + DP(i - j, j);
                } else {
                    DP(i, j) = DP(i, j-1);
                }
            }
        }
        #undef DP
    }

    /* The answer is the sum over all k of p(n - minimal_sum_k, k)            */
    seq_t  result = 0;
    for (idx_t k = 1; k <= kmax_needed; ++k) {
        /* minimal_sum_k = k*(k^2+1)/2  */
        unsigned __int128 k128 = k;
        unsigned __int128 min_sum128 = k128 * (k128 * k128 + 1) / 2;
        idx_t  min_sum = (idx_t)min_sum128;   /* fits within idx_t for n<=~10^12 */
        if (min_sum > n) break;
        idx_t  residue = n - min_sum;
        /* dp[residue][k] is the number of partitions of residue with parts ≤k  */
        /* addition with overflow safe, seq_t handles big integers             */
        result += dp[residue * dp_cols + k];
    }

    return result;
}
