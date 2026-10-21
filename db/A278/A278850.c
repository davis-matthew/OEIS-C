/*
 * A278850  Number of nX3 0..1 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly three mistakes.
 * https://oeis.org/A278850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 44, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP counting of n×3 binary matrices with rows nondecreasing and columns
   nonincreasing, allowing exactly three violations (mistakes).
   State: last row value (0..7), column1–2 state (0=equal,1=good,2=bad),
   column2–3 state (same), and total mistakes so far.
   Transitions add a new row, update row mistakes and first‐difference
   column states as described. Final answer is the sum of states with
   total mistakes == 3. */
#include <stdint.h>
#include <stdbool.h>

#define ROWS 8
#define STATE 3
#define MIS 4

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* dp[last_row][cs12][cs23][mistakes] */
    seq_t dp[ROWS][STATE][STATE][MIS];
    seq_t next[ROWS][STATE][STATE][MIS];
    for (int r = 0; r < ROWS; ++r)
        for (int c12 = 0; c12 < STATE; ++c12)
            for (int c23 = 0; c23 < STATE; ++c23)
                for (int m = 0; m < MIS; ++m)
                    dp[r][c12][c23][m] = 0, next[r][c12][c23][m] = 0;

    /* initialization with first row */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        int c1 = (r0 >> 2) & 1;
        int c2 = (r0 >> 1) & 1;
        int c3 = r0 & 1;

        int cs12 = 0, cs23 = 0, mistakes = 0;
        if (c1 > c2) cs12 = 1;
        else if (c1 < c2) { cs12 = 2; ++mistakes; }

        if (c2 > c3) cs23 = 1;
        else if (c2 < c3) { cs23 = 2; ++mistakes; }

        dp[r0][cs12][cs23][mistakes] = (seq_t)1;
    }

    /* Process remaining rows */
    for (idx_t step = 2; step <= n; ++step) {
        /* reset next */
        for (int r = 0; r < ROWS; ++r)
            for (int c12 = 0; c12 < STATE; ++c12)
                for (int c23 = 0; c23 < STATE; ++c23)
                    for (int m = 0; m < MIS; ++m)
                        next[r][c12][c23][m] = 0;

        for (int prev = 0; prev < ROWS; ++prev) {
            int pc1 = (prev >> 2) & 1;
            int pc2 = (prev >> 1) & 1;
            int pc3 = prev & 1;
            (void)pc1; (void)pc2; (void)pc3; /* not needed */

            for (int cs12 = 0; cs12 < STATE; ++cs12)
                for (int cs23 = 0; cs23 < STATE; ++cs23)
                    for (int m = 0; m < MIS; ++m) {
                        seq_t cur = dp[prev][cs12][cs23][m];
                        if (cur == (seq_t)0) continue;

                        for (int rnext = 0; rnext < ROWS; ++rnext) {
                            int nc1 = (rnext >> 2) & 1;
                            int nc2 = (rnext >> 1) & 1;
                            int nc3 = rnext & 1;

                            /* row mistake */
                            int addmis = (prev > rnext) ? 1 : 0;

                            /* column 1-2 */
                            int ns12 = cs12;
                            if (cs12 == 0) {
                                if (nc1 > nc2) ns12 = 1;
                                else if (nc1 < nc2) { ns12 = 2; ++addmis; }
                            }

                            /* column 2-3 */
                            int ns23 = cs23;
                            if (cs23 == 0) {
                                if (nc2 > nc3) ns23 = 1;
                                else if (nc2 < nc3) { ns23 = 2; ++addmis; }
                            }

                            int newm = m + addmis;
                            if (newm <= 3)
                                next[rnext][ns12][ns23][newm] += cur;
                        }
                    }
        }

        /* move next to dp */
        for (int r = 0; r < ROWS; ++r)
            for (int c12 = 0; c12 < STATE; ++c12)
                for (int c23 = 0; c23 < STATE; ++c23)
                    for (int m = 0; m < MIS; ++m)
                        dp[r][c12][c23][m] = next[r][c12][c23][m];
    }

    /* sum states with exactly 3 mistakes */
    seq_t ans = 0;
    for (int r = 0; r < ROWS; ++r)
        for (int c12 = 0; c12 < STATE; ++c12)
            for (int c23 = 0; c23 < STATE; ++c23)
                ans += dp[r][c12][c23][3];

    return ans;
}
