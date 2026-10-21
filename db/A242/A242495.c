/*
 * A242495  Number of length n words on {1,2,3,4} with at most one consecutive
 *          1 and at most two consecutive 2's and at most three consecutive
 *          3's and at most four consecutive 4's.
 * https://oeis.org/A242495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (via DP over the last symbol and its run length):
   a(0) = 1.
   For n ≥ 1, let dp[s][r] be the number of words of length n that end
   with symbol s (0 for 1, 1 for 2, 2 for 3, 3 for 4) and a run
   of length r (1 ≤ r ≤ max_run[s]).
   Transitions from length n−1 to n:
     • If r < max_run[s]  →  dp'[s][r+1] += dp[s][r]      (continue same symbol)
     • For every t ≠ s → dp'[t][1] += dp[s][r]          (start new symbol)
   Then a(n) = Σ_{s,r} dp[s][r].
   This matches OEIS A242495. */
seq_t a(idx_t n)
{
    static const int offset[4] = {0, 1, 3, 6};
    static const int max_run[4] = {1, 2, 3, 4};
    static const int state_sym[10] = {0, 1, 1, 2, 2, 2, 3, 3, 3, 3};
    static const int state_run[10] = {1, 1, 2, 1, 2, 3, 1, 2, 3, 4};

    if (n == 0) return (seq_t)1;

    seq_t dp_cur[10] = {0};
    seq_t dp_next[10];
    int s;

    for (idx_t len = 0; len < n; ++len) {
        for (s = 0; s < 10; ++s) dp_next[s] = 0;

        if (len == 0) {                     /* first symbol */
            for (int sym = 0; sym < 4; ++sym)
                dp_next[offset[sym]] = 1;
        } else {
            for (s = 0; s < 10; ++s) {
                seq_t val = dp_cur[s];
                if (!val) continue;
                int sym = state_sym[s];
                int run = state_run[s];

                if (run < max_run[sym]) {    /* continue same symbol */
                    int idx = offset[sym] + (run + 1) - 1;
                    dp_next[idx] += val;
                }

                for (int sym2 = 0; sym2 < 4; ++sym2) {  /* start new symbol */
                    if (sym2 == sym) continue;
                    int idx = offset[sym2];
                    dp_next[idx] += val;
                }
            }
        }

        for (s = 0; s < 10; ++s) dp_cur[s] = dp_next[s];
    }

    seq_t total = 0;
    for (s = 0; s < 10; ++s) total += dp_cur[s];
    return total;
}
