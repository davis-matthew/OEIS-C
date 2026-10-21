/*
 * A278849  Number of nX2 0..1 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly three mistakes.
 * https://oeis.org/A278849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 binary matrices with rows in nondecreasing
 *            lexicographic order, columns in nonincreasing lexicographic
 *            order, and exactly three mistakes (adjacent row inversions plus
 *            a column inversion)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 23, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* dp[v][m][c] : number of prefixes ending with value v (0..3),
                     having m row‑mistakes (0..3),
                     column comparison state c (0=equal so far,
                                                1=high>low already,
                                                2=high<low already). */
    seq_t dp[4][4][3];
    seq_t ndp[4][4][3];
    int i, j, k;

    /* zero dp */
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            for (k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    /* initialize for the first row */
    for (i = 0; i < 4; ++i) {
        int h = i >> 1;          /* high bit */
        int l = i & 1;           /* low  bit */
        int c = (h == l) ? 0 : (h > l ? 1 : 2);
        dp[i][0][c] = (seq_t)1;
    }

    /* iterate over remaining rows */
    for (idx_t len = 2; len <= n; ++len) {
        /* zero ndp */
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                for (k = 0; k < 3; ++k)
                    ndp[i][j][k] = 0;

        for (int pv = 0; pv < 4; ++pv) {          /* previous value */
            for (int m = 0; m < 4; ++m) {
                for (int cprev = 0; cprev < 3; ++cprev) {
                    seq_t cnt = dp[pv][m][cprev];
                    if (cnt == 0) continue;

                    for (int nv = 0; nv < 4; ++nv) {  /* new value */
                        int inc = (pv > nv) ? 1 : 0;
                        int m2 = m + inc;
                        if (m2 > 3) continue;

                        int c2 = cprev;
                        if (cprev == 0) {
                            int h = nv >> 1;
                            int l = nv & 1;
                            if (h == l)
                                c2 = 0;
                            else
                                c2 = (h > l) ? 1 : 2;
                        }
                        ndp[nv][m2][c2] += cnt;
                    }
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                for (k = 0; k < 3; ++k)
                    dp[i][j][k] = ndp[i][j][k];
    }

    /* sum sequences with total mistakes = 3 */
    seq_t total = 0;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            for (k = 0; k < 3; ++k)
                if (j + (k == 2 ? 1 : 0) == 3)
                    total += dp[i][j][k];

    return total;
}
