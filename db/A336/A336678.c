/*
 * A336678  Number of paths of length n starting at initial node of the path
 *          graph P_11.
 * https://oeis.org/A336678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = sum_{i=0}^{10} dp_i(n), with dp_0(0)=1, dp_i(0)=0 (i>0)
 *            dp_0(k+1)=dp_1(k) dp_i(k+1)=dp_{i-1}(k)+dp_{i+1}(k) for 0<i<10
 *            dp_{10}(k+1)=dp_9(k) This counts length‑n walks on the path
 *            graph P_11 starting from the endpoint
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t M = 11;          /* number of vertices in P_11 */

    seq_t dp[M];
    seq_t ndp[M];
    idx_t step, i;

    /* initialise dp for step 0 */
    for (i = 0; i < M; ++i) dp[i] = 0;
    dp[0] = 1;

    for (step = 0; step < n; ++step) {
        /* vertex 0 receives walks only from vertex 1 */
        ndp[0] = dp[1];

        /* interior vertices */
        for (i = 1; i + 1 < M; ++i)
            ndp[i] = dp[i - 1] + dp[i + 1];

        /* last vertex receives walks only from vertex M-2 */
        ndp[M - 1] = dp[M - 2];

        /* copy back for next iteration */
        for (i = 0; i < M; ++i)
            dp[i] = ndp[i];
    }

    /* total walks after n steps = sum of counts at all vertices */
    seq_t sum = 0;
    for (i = 0; i < M; ++i)
        sum += dp[i];

    return sum;
}
