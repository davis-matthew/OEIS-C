/*
 * A383071  Number of n-digit positive integers where all pairs of consecutive
 *          digits have a difference of at least 3.
 * https://oeis.org/A383071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over digit states.
   For each n, let dp[d] be the number of length-i sequences that end
   with digit d (0 ≤ d ≤ 9).  Initial state (i = 1):
      dp[d] = 1 for d = 1…9, 0 for d = 0.
   Transition:
      next[d] = Σ_{p: |d - p| ≥ 3} dp[p]
   a(n) = Σ_{d=0}^{9} dp[d] after n steps. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* dp arrays for previous and current step */
    seq_t dp_prev[10], dp_curr[10];
    int d;

    /* initialize dp_prev for length 1 */
    for (d = 0; d < 10; ++d)
        dp_prev[d] = (d == 0) ? (seq_t)0 : (seq_t)1;

    /* iterate for lengths 2 .. n */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        for (d = 0; d < 10; ++d) {
            seq_t sum = 0;
            int pd;
            for (pd = 0; pd < 10; ++pd) {
                int diff = d - pd;
                if (diff < 0)
                    diff = -diff;
                if (diff >= 3)
                    sum += dp_prev[pd];
            }
            dp_curr[d] = sum;
        }
        /* copy dp_curr to dp_prev for next iteration */
        for (d = 0; d < 10; ++d)
            dp_prev[d] = dp_curr[d];
    }

    /* sum all ending states to get the count */
    seq_t total = 0;
    for (d = 0; d < 10; ++d)
        total += dp_prev[d];

    return total;
}
