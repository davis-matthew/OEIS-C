/*
 * A297766  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 4 neighboring 1s.
 * https://oeis.org/A297766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of A297766 (number of 5×n 0..1 matrices where each 1
 *  has 2–4 neighboring 1's vertically, horizontally, or along the
 *  two diagonals).  The algorithm implements a dynamic programming
 *  over 5‑bit column patterns.  The definition follows the OEIS
 *  description directly; no closed formula is known.
 */

#include <stdbool.h>

#define STATES 32          /* 5‑bit column patterns 0 … 31 */

/* ------------------------------------------------------------------- */
/*  Main routine – returns the nth term (a(1) = 1).                     */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Pre‑computed table: good[p][c][q] is true iff column c
     * satisfies the neighbor constraint with neighbors p (left)
     * and q (right).  The table is computed once per program. */
    static bool good[STATES][STATES][STATES];
    static bool init = false;

    if (!init) {
        for (int p = 0; p < STATES; ++p) {
            for (int c = 0; c < STATES; ++c) {
                for (int q = 0; q < STATES; ++q) {
                    bool ok = true;
                    for (int r = 0; r < 5; ++r) {        /* row r in center column */
                        int bit = (c >> r) & 1;
                        if (!bit) continue;              /* zeros impose no rule */
                        int cnt = 0;
                        for (int dr = -1; dr <= 1; ++dr) {/* adjacent rows in neighbors */
                            int rr = r + dr;
                            if (0 <= rr && rr < 5) {
                                cnt += (p >> rr) & 1;
                                cnt += (q >> rr) & 1;
                            }
                        }
                        if (cnt < 2 || cnt > 4) { ok = false; break; }
                    }
                    good[p][c][q] = ok;
                }
            }
        }
        init = true;
    }

    /* Dynamic programming tables: dp[prev][curr] and dp_next[prev][curr]. */
    static seq_t dp[STATES][STATES];
    static seq_t dp_next[STATES][STATES];

    /* Zero the DP tables for the new call. */
    for (int p = 0; p < STATES; ++p)
        for (int c = 0; c < STATES; ++c)
            dp[p][c] = 0;

    /* Initial state: the left border is a virtual column of zeros. */
    for (int s = 0; s < STATES; ++s)
        dp[0][s] = 1;

    /* Process columns 2 … n.  n == 1 causes the loop to be skipped. */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero dp_next for this iteration. */
        for (int p = 0; p < STATES; ++p)
            for (int c = 0; c < STATES; ++c)
                dp_next[p][c] = 0;

        for (int p = 0; p < STATES; ++p) {
            for (int c = 0; c < STATES; ++c) {
                seq_t val = dp[p][c];
                if (!val) continue;
                for (int q = 0; q < STATES; ++q) {
                    if (good[p][c][q])
                        dp_next[c][q] += val;
                }
            }
        }

        /* Copy dp_next into dp for the next step. */
        for (int p = 0; p < STATES; ++p)
            for (int c = 0; c < STATES; ++c)
                dp[p][c] = dp_next[p][c];
    }

    /* Sum over final states where the last column satisfies the right
     * border condition with a virtual column of zeros. */
    seq_t result = 0;
    for (int p = 0; p < STATES; ++p) {
        for (int c = 0; c < STATES; ++c) {
            if (good[p][c][0])
                result += dp[p][c];
        }
    }
    return result;
}
