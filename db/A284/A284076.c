/*
 * A284076  Number of 2Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A284076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 2×n 0..1 arrays with no 1 having more than three of
 *            its horizontal, diagonal and antidiagonal neighbors, except that
 *            exactly two elements may violate the rule
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* DP state: [prev][curr][violations so far]
       prev, curr are 2‑bit masks for the two rows of a column
       violations may be 0, 1 or 2 (we never need more).             */
    static seq_t dp[4][4][3];
    static seq_t ndp[4][4][3];

    /* initialise dp for a dummy left column of zeros */
    for (int p = 0; p < 4; ++p)
        for (int c = 0; c < 4; ++c)
            for (int v = 0; v < 3; ++v)
                dp[p][c][v] = 0;
    for (int c = 0; c < 4; ++c)
        dp[0][c][0] = (seq_t)1;    /* prev dummy column = 0 */

    /* process columns from 1 to n, with a dummy right column at the end */
    for (idx_t i = 2; i <= n + 1; ++i) {
        /* clear ndp */
        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b)
                for (int v = 0; v < 3; ++v)
                    ndp[a][b][v] = 0;

        for (int prev = 0; prev < 4; ++prev) {
            for (int curr = 0; curr < 4; ++curr) {
                for (int v = 0; v < 3; ++v) {
                    seq_t ways = dp[prev][curr][v];
                    if (!ways)
                        continue;

                    for (int next = 0; next < 4; ++next) {
                        int viol = 0;

                        /* top row cell */
                        if (curr & 1) {
                            int cnt = ((prev >> 0) & 1) + ((next >> 0) & 1) +
                                      ((prev >> 1) & 1) + ((next >> 1) & 1);
                            if (cnt > 3)
                                ++viol;
                        }

                        /* bottom row cell */
                        if (curr & 2) {
                            int cnt = ((prev >> 1) & 1) + ((next >> 1) & 1) +
                                      ((prev >> 0) & 1) + ((next >> 0) & 1);
                            if (cnt > 3)
                                ++viol;
                        }

                        int nv = v + viol;
                        if (nv > 2)
                            continue;

                        ndp[curr][next][nv] += ways;
                    }
                }
            }
        }

        /* copy ndp back to dp for the next iteration */
        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b)
                for (int v = 0; v < 3; ++v)
                    dp[a][b][v] = ndp[a][b][v];
    }

    /* after the last transition, the dummy right column is 0 */
    seq_t result = 0;
    for (int prev = 0; prev < 4; ++prev)
        result += dp[prev][0][2];

    return result;
}
