/*
 * A278622  Number of nX3 0..2 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly one mistake.
 * https://oeis.org/A278622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of n×3 arrays with entries 0..2 whose
 *            rows are in nondecreasing lexicographic order and columns are in
 *            nonincreasing lexicographic order, with exactly one violation of
 *            either type. The values are computed by dynamic programming over
 *            the rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 28, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */

    /* Precompute the 27 possible rows (triples in base 3). */
    int d[27][3];
    for (int r = 0; r < 27; ++r) {
        d[r][0] = r / 9;          /* most significant digit */
        d[r][1] = (r / 3) % 3;
        d[r][2] = r % 3;          /* least significant digit */
    }

    /* Precompute row lexicographic comparison table. */
    bool leq[27][27];
    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < 27; ++j) {
            bool ok = true;
            for (int k = 0; k < 3; ++k) {
                if (d[i][k] < d[j][k]) { ok = true; break; }
                if (d[i][k] > d[j][k]) { ok = false; break; }
                /* else equal -> continue */
            }
            leq[i][j] = ok;
        }
    }

    /* Dynamic programming arrays.
       State: previous row index, column1>col2 status, column2>col3 status, mistakes used (0 or 1). */
    seq_t dp[27][3][3][2];
    seq_t ndp[27][3][3][2];

    /* Initialize to zero */
    for (int i = 0; i < 27; ++i)
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int m = 0; m < 2; ++m)
                    dp[i][a][b][m] = 0;

    /* First row: no row comparison yet. */
    for (int r = 0; r < 27; ++r) {
        int c12 = 0, c23 = 0;   /* 0 = undecided, 1 = satisfied, 2 = violation */
        int m = 0;
        int diff1 = d[r][0] - d[r][1];
        if (diff1 > 0)           c12 = 1;
        else if (diff1 < 0) {    c12 = 2; m++; }
        int diff2 = d[r][1] - d[r][2];
        if (diff2 > 0)           c23 = 1;
        else if (diff2 < 0) {    c23 = 2; m++; }
        if (m <= 1) {
            dp[r][c12][c23][m] += (seq_t)1;
        }
    }

    /* Process remaining rows */
    for (idx_t pos = 1; pos < n; ++pos) {
        /* Zero next dp */
        for (int i = 0; i < 27; ++i)
            for (int a = 0; a < 3; ++a)
                for (int b = 0; b < 3; ++b)
                    for (int m = 0; m < 2; ++m)
                        ndp[i][a][b][m] = 0;

        for (int prev = 0; prev < 27; ++prev)
            for (int s12 = 0; s12 < 3; ++s12)
                for (int s23 = 0; s23 < 3; ++s23)
                    for (int m = 0; m < 2; ++m) {
                        seq_t cur = dp[prev][s12][s23][m];
                        if (!cur) continue;

                        for (int next = 0; next < 27; ++next) {
                            int new_m = m;
                            /* Row comparison: prev <= next? */
                            if (!leq[prev][next]) {
                                new_m++;
                                if (new_m > 1) continue;
                            }

                            int ns12 = s12;
                            int ns23 = s23;

                            /* Column1 vs Column2 status update */
                            if (s12 == 0) {
                                int diff = d[next][0] - d[next][1];
                                if (diff > 0) ns12 = 1;
                                else if (diff < 0) {
                                    ns12 = 2;
                                    new_m++;
                                    if (new_m > 1) continue;
                                }
                            }

                            /* Column2 vs Column3 status update */
                            if (s23 == 0) {
                                int diff = d[next][1] - d[next][2];
                                if (diff > 0) ns23 = 1;
                                else if (diff < 0) {
                                    ns23 = 2;
                                    new_m++;
                                    if (new_m > 1) continue;
                                }
                            }

                            ndp[next][ns12][ns23][new_m] += cur;
                        }
                    }

        /* Copy ndp to dp for next iteration */
        for (int i = 0; i < 27; ++i)
            for (int a = 0; a < 3; ++a)
                for (int b = 0; b < 3; ++b)
                    for (int m = 0; m < 2; ++m)
                        dp[i][a][b][m] = ndp[i][a][b][m];
    }

    /* Sum all states with exactly one mistake. */
    seq_t ans = 0;
    for (int r = 0; r < 27; ++r)
        for (int s12 = 0; s12 < 3; ++s12)
            for (int s23 = 0; s23 < 3; ++s23)
                ans += dp[r][s12][s23][1];

    return ans;
}
