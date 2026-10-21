/*
 * A277670  Number of n-length words over a 7-ary alphabet {a_1,a_2,...,a_7}
 *          avoiding consecutive letters a_i, a_{i+1}.
 * https://oeis.org/A277670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A277670: Number of n‑length words over a 7‑ary alphabet
 * avoiding consecutive letters a_i, a_{i+1}.
 *
 * The sequence satisfies:
 *   a(0) = 1,
 *   a(1) = 7;
 *   for n ≥ 2:
 *     let dp[n][i] (i = 0..6) be the number of words of length n
 *     ending with letter a_{i+1}.
 *     dp[1][i] = 1,
 *     dp[n][i] = Σ_{j≠i-1} dp[n-1][j]   (i>0)  or  Σ_{j} dp[n-1][j] (i=0)
 *     a(n) = Σ_{i=0}^{6} dp[n][i].
 *
 * The implementation uses a simple dynamic programming loop.
 */

seq_t a(idx_t n)
{
    /* Special case: empty word */
    if (n == 0) {
        return (seq_t)1;
    }

    /* DP arrays for one step */
    seq_t prev[7];
    seq_t cur[7];
    seq_t total_prev = 7;   /* sum of prev[0..6] for length 1 */

    /* Initialize for length 1 */
    for (idx_t i = 0; i < 7; ++i) {
        prev[i] = (seq_t)1;
    }

    /* Iterate lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        seq_t total_cur = 0;

        /* Letter a1 (index 0) has no restriction */
        cur[0] = total_prev;
        total_cur += cur[0];

        /* Letters a2..a7 (indices 1..6) forbid previous letter i-1 */
        for (idx_t i = 1; i < 7; ++i) {
            cur[i] = total_prev - prev[i - 1];
            total_cur += cur[i];
        }

        /* Prepare for next iteration */
        for (idx_t i = 0; i < 7; ++i) {
            prev[i] = cur[i];
        }
        total_prev = total_cur;
    }

    /* total_prev now equals a(n) */
    return total_prev;
}
