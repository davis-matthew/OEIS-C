/*
 * A250827  Number of (n+1) X (1+1) 0..4 arrays with nondecreasing maximum of
 *          every two consecutive values in every row and column.
 * https://oeis.org/A250827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250827: a(n) = f(n)^2 where f(n) counts sequences of length n+1 over {0..4}
 * such that the maximum of every pair of consecutive entries is
 * nondecreasing.  f(n) is computed by a dynamic‑programming recurrence. */

seq_t a(idx_t n)
{
    /* f is the number of admissible column sequences */
    seq_t f = 0;
    /* dp[v][M] – number of sequences ending with value v and last
     * maximum M (>= v).  M ranges 0..4, but we store all 5×5 states
     * for simplicity. */
    seq_t dp[5][5] = {0};
    seq_t ndp[5][5] = {0};

    /* Initialise with all 2‑element sequences (length 2). */
    for (int x = 0; x <= 4; ++x) {
        for (int y = 0; y <= 4; ++y) {
            int M = (x > y) ? x : y;
            dp[y][M] += 1;
        }
    }

    /* For n == 1 we already have all length‑2 sequences. */
    if (n > 1) {
        /* Extend sequences to length n+1.  We perform n−1 extensions. */
        for (idx_t step = 2; step <= n; ++step) {
            /* zero ndp */
            for (int i = 0; i < 5; ++i)
                for (int j = 0; j < 5; ++j)
                    ndp[i][j] = 0;

            /* transition from dp to ndp */
            for (int v = 0; v <= 4; ++v) {
                for (int M = 0; M <= 4; ++M) {
                    seq_t val = dp[v][M];
                    if (!val) continue;
                    for (int w = 0; w <= 4; ++w) {
                        int M2 = (v > w) ? v : w;   /* new max */
                        if (M <= M2) ndp[w][M2] += val;
                    }
                }
            }

            /* copy ndp back to dp for next iteration */
            for (int i = 0; i < 5; ++i)
                for (int j = 0; j < 5; ++j)
                    dp[i][j] = ndp[i][j];
        }
    }

    /* sum all states to get f(n) */
    for (int v = 0; v <= 4; ++v)
        for (int M = 0; M <= 4; ++M)
            f += dp[v][M];

    /* a(n) = f(n)^2 */
    return f * f;
}
