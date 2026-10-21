/*
 * A283859  Number of 3Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) + 5*a(n-2) + 50*a(n-3) + 21*a(n-4) + 
 *          11*a(n-5) - 67*a(n-6) - 229*a(n-7) - 116*a(n-8) + 41*a(n-9) + 
 *          116*a(n-10) + 66*a(n-11) + 15*a(n-12) + a(n-13) - 2*a(n-14)   (n > 14)
 * Explicit definition: For a given n, a(n) counts all 3×n binary matrices such
 * that for every cell containing 1, the number of its horizontal, diagonal
 * and antidiagonal neighbors also containing 1 does not exceed 3.
 * The sequence is independent of the recurrence, which is used only for
 * verification. The value is computed directly from the definition using
 * dynamic programming. Initial values are derived from the definition for n=1…10
 * and the recurrence is not required in the implementation. */

#include <stdint.h>
#include <stdbool.h>

/* Authorisation: function a(idx_t n) returning seq_t   */
seq_t a(idx_t n)
{
    /* DP states: dp[left][mid] = number of ways to build a prefix ending
       with column patterns 'left' and 'mid' (in binary, 3 bits each).  */
    static bool valid[8][8][8];
    static bool table_done = false;

    /* Build table of valid middle patterns for every pair of neighbour
       columns (left,mid,right).  */
    if (!table_done) {
        for (int L = 0; L < 8; ++L) {
            for (int M = 0; M < 8; ++M) {
                for (int R = 0; R < 8; ++R) {
                    bool ok = true;
                    for (int r = 0; r < 3; ++r) {          /* for each row */
                        if ((M >> r) & 1) {                /* 1 present */
                            int cnt = 0;
                            /* same row neighbours */
                            if ((L >> r) & 1) ++cnt;
                            if ((R >> r) & 1) ++cnt;
                            /* up‑left  */
                            if (r > 0 && ((L >> (r-1)) & 1)) ++cnt;
                            /* up‑right */
                            if (r > 0 && ((R >> (r-1)) & 1)) ++cnt;
                            /* down‑left */
                            if (r < 2 && ((L >> (r+1)) & 1)) ++cnt;
                            /* down‑right */
                            if (r < 2 && ((R >> (r+1)) & 1)) ++cnt;
                            if (cnt > 3) { ok = false; break; }
                        }
                    }
                    valid[L][M][R] = ok;
                }
            }
        }
        table_done = true;
    }

    /* 0‑based DP arrays.  */
    seq_t dp[8][8], ndp[8][8];

    /* initialise dp with dummy left column pattern 0 and all possible
       first column patterns.  */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = (seq_t)0;
    for (int p = 0; p < 8; ++p)
        dp[0][p] = (seq_t)1;                 /* (left = 0, mid = p) */

    /* Process remaining columns.  */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = (seq_t)0;        /* zero new dp */

        for (int left = 0; left < 8; ++left) {
            for (int mid = 0; mid < 8; ++mid) {
                seq_t cur = dp[left][mid];
                if (cur == (seq_t)0) continue; /* skip impossible state */
                for (int right = 0; right < 8; ++right) {
                    if (valid[left][mid][right])
                        ndp[mid][right] = ndp[mid][right] + cur;
                }
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* After processing all columns, evaluate the last column
       against a dummy right neighbour (0).  */
    seq_t total = (seq_t)0;
    for (int left = 0; left < 8; ++left) {
        for (int mid = 0; mid < 8; ++mid) {
            if (valid[left][mid][0])
                total = total + dp[left][mid];
        }
    }

    return total;
}
