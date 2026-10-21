/*
 * A269914  Number of ternary strings of length n with maximal run length two
 *          containing 112.
 * https://oeis.org/A269914
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts ternary strings of length n with maximal run length two that contain the substring 112.
   The function works for all n ≥ 3; for n < 3 it returns 0 (undefined behaviour below the offset). */
seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return 0. */
    if (n < 3) {
        return 0;
    }

    /* dp[d1][d2][s] = number of strings of current length whose last two digits are
       (d1,d2) and where s==1 means that the substring 112 has already appeared. */
    seq_t dp[4][4][2];
    seq_t ndp[4][4][2];
    int d1, d2, se;

    /* initialise dp */
    for (d1 = 0; d1 < 4; ++d1)
        for (d2 = 0; d2 < 4; ++d2)
            for (se = 0; se < 2; ++se)
                dp[d1][d2][se] = 0;
    dp[3][3][0] = 1;   /* empty string: both last digits undefined (sentinel 3) */

    for (idx_t step = 0; step < n; ++step) {

        /* zero ndp */
        for (d1 = 0; d1 < 4; ++d1)
            for (d2 = 0; d2 < 4; ++d2)
                for (se = 0; se < 2; ++se)
                    ndp[d1][d2][se] = 0;

        for (d1 = 0; d1 < 4; ++d1) {
            for (d2 = 0; d2 < 4; ++d2) {
                for (se = 0; se < 2; ++se) {
                    seq_t cnt = dp[d1][d2][se];
                    if (!cnt) continue;

                    for (int d = 0; d < 3; ++d) {
                        /* Skip if adding d would create a run of 3 equal digits. */
                        if (d1 != 3 && d2 != 3 && d1 == d2 && d2 == d) {
                            continue;
                        }

                        int nd1 = d2;      /* new last two digits */
                        int nd2 = d;
                        int nse = se;

                        /* Mark that we have seen 112 if not already seen. */
                        if (!se && d1 == 1 && d2 == 1 && d == 2) {
                            nse = 1;
                        }

                        ndp[nd1][nd2][nse] += cnt;
                    }
                }
            }
        }

        /* copy ndp into dp for next iteration */
        for (d1 = 0; d1 < 4; ++d1)
            for (d2 = 0; d2 < 4; ++d2)
                for (se = 0; se < 2; ++se)
                    dp[d1][d2][se] = ndp[d1][d2][se];
    }

    /* Sum over all states where the substring 112 has been seen. */
    seq_t result = 0;
    for (d1 = 0; d1 < 4; ++d1)
        for (d2 = 0; d2 < 4; ++d2)
            result += dp[d1][d2][1];

    return result;
}
