/*
 * A384497  a(n) is the number of binary strings of length n which contain
 *          exactly one run of 1s of even length.
 * https://oeis.org/A384497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of binary strings of length n containing exactly one run
 *            of 1's of even length. The function computes this value by
 *            dynamic programming over the string positions, tracking: -
 *            whether the parser is currently inside a run of 1's, - the
 *            parity (even/odd) of the current run length if inside, -...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types are provided by the build */
seq_t a(idx_t n)
{
    /* DP state: dp[in_run][run_parity][seen_even] */
    seq_t dp[2][2][2] = {0};
    seq_t next[2][2][2] = {0};

    dp[0][0][0] = 1;               /* empty string: not in run, no even run seen */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* zero the next array */
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    next[i][j][k] = 0;

        for (int in_run = 0; in_run < 2; ++in_run) {
            for (int run_par = 0; run_par < 2; ++run_par) {
                for (int seen = 0; seen < 2; ++seen) {
                    seq_t val = dp[in_run][run_par][seen];
                    if (val == 0) continue;

                    /* transition by adding a 0 */
                    if (in_run == 1) {
                        /* finishing current run */
                        if (!(run_par == 0 && seen == 1)) {   /* avoid second even run */
                            int new_seen = seen | (run_par == 0);
                            next[0][0][new_seen] += val;
                        }
                    } else {
                        /* remain not in run */
                        next[0][0][seen] += val;
                    }

                    /* transition by adding a 1 */
                    if (in_run == 1) {
                        int new_par = run_par ^ 1;
                        next[1][new_par][seen] += val;
                    } else {
                        /* start a new run (odd length 1) */
                        next[1][1][seen] += val;
                    }
                }
            }
        }

        /* swap dp and next for next position */
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    dp[i][j][k] = next[i][j][k];
    }

    /* Sum states that correspond to exactly one even run */
    seq_t ans = 0;
    /* ended in non-run state, already seen one even run */
    ans += dp[0][0][1];
    /* ended inside an odd run, seen one even run before */
    ans += dp[1][1][1];
    /* ended inside an even run, no even run seen before */
    ans += dp[1][0][0];

    return ans;
}
