/*
 * A385870  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 1 or 6.
 * https://oeis.org/A385870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 53, u64 n <= 107,
 *            u128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of subsets of {1,2,…,n} with no two elements differing by 1 or 6.
   The algorithm uses dynamic programming over a 6‑bit state that encodes the
   selection status of the previous 6 positions.  For each new position it
   considers selecting or skipping it, updating the state accordingly.
   The total is the sum of all state counts after processing n. */
seq_t a(idx_t n)
{
    /* 64 possible states for the last six positions */
    seq_t dp[64]  = {0};
    seq_t ndp[64] = {0};
    dp[0] = 1;                         /* start with no previous selections */

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear next-state array */
        for (int i = 0; i < 64; ++i) ndp[i] = 0;

        for (int state = 0; state < 64; ++state) {
            seq_t ways = dp[state];
            if (!ways) continue;

            /* Case 1: do not select current position */
            int new_state = (state << 1) & 0x3E;   /* shift left, drop bit 5, set bit 0 = 0 */
            ndp[new_state] += ways;

            /* Case 2: select current position, allowed only if previous pos and pos-6 are not selected */
            if ((state & 1) == 0 && (state & 0x20) == 0) {
                int picked_state = new_state | 1;   /* set new bit 0 to 1 */
                ndp[picked_state] += ways;
            }
        }

        /* move to next position */
        for (int i = 0; i < 64; ++i) dp[i] = ndp[i];
    }

    /* sum all state counts to obtain a(n) */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i) total += dp[i];
    return total;
}
