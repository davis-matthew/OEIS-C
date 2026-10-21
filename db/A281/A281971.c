/*
 * A281971  Number of sets of exactly n positive integers <= n+8 having a
 *          square element sum.
 * https://oeis.org/A281971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n-element subsets of {1,…,n+8} whose element
 *            sum is a perfect square
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts, for each M = n+8, the 8‑element subsets of {1,…,M} whose sum
 * equals S_total - k² for any integer k, where S_total = M(M+1)/2. */
/* It employs incremental dynamic programming over the 8 smallest elements. */

#include <stdlib.h>

#define MAX_N 20000          /* maximum n expected; upgrade if larger n needed */
#define MAX_M (MAX_N + 8)    /* maximum M = n + 8 */
#define MAX_SUM (8 * MAX_M - 28)  /* maximum sum for 8 distinct numbers in 1..MAX_M */

static seq_t dp[9][MAX_SUM + 1];  /* dp[s][sum] = number of s‑element subsets of processed numbers with sum 'sum' */
static idx_t processed_M = 0;     /* largest M processed so far */

seq_t a(idx_t n) {
    /* convert n to M */
    idx_t M = n + 8;
    if (M > MAX_M) {
        /* out of bounds of preallocated tables – should not happen for typical use */
        return (seq_t)0;
    }

    /* Initialize dp[0][0] on first use */
    if (processed_M == 0) {
        dp[0][0] = (seq_t)1;
    }

    /* Incrementally update dp up to the desired M */
    for (idx_t i = processed_M + 1; i <= M; ++i) {
        /* Update for subset sizes from min(i,8) down to 1 */
        for (idx_t s = 8; s >= 1; --s) {
            if (s > i) continue;          /* cannot choose more elements than processed numbers */
            int maxS = (int)(s * (int)i - s * (s - 1) / 2);  /* maximum possible sum for size s up to i */
            for (int sum = maxS; sum >= (int)i; --sum) {
                dp[s][sum] += dp[s - 1][sum - (int)i];
            }
        }
    }
    processed_M = M;

    /* Compute total sum of 1..M */
    idx_t S_total = M * (M + 1) / 2;

    /* Count 8‑element subsets whose sum equals S_total - k² for some k. */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; ; ++k) {
        idx_t k2 = k * k;
        if (k2 > S_total) break;
        idx_t t = S_total - k2;          /* required sum of the 8‑element subset */
        if (t <= MAX_SUM) {
            result += dp[8][(int)t];
        }
    }
    return result;
}
