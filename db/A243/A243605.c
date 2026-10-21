/*
 * A243605  Number of length n+2 0..8 arrays with no three elements in a row
 *          with pattern aba (with a!=b) and new values 0..8 introduced in
 *          0..8 order.
 * https://oeis.org/A243605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 arrays over 0..8 with no three
 *            consecutive elements of the form a b a (a≠b), and alphabet
 *            values introduced in order (use 0 first, then 1, … ). Computed
 *            by dynamic programming on the last two elements and the current
 *            maximum value used
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 we return 0. */
    if (n < 1) return 0;

    /* Length of the arrays under consideration. */
    const idx_t L = n + 2;          /* L ≥ 3 for n ≥ 1 */

    /* DP state: dp[p1][p2][m] = number of arrays of current length
       whose last two elements are (p1, p2) and whose maximum value so far is m.
       Each of p1, p2, m ranges over 0..8 (9 values). */
    static seq_t dp[9][9][9];
    static seq_t dp_next[9][9][9];

    /* Initialize to zero. */
    memset(dp, 0, sizeof(dp));

    /* Length 2: first element must be 0.
       Second element can be 0 or introduce the new value 1. */
    dp[0][0][0] = 1;      /* array [0,0] */
    dp[0][1][1] = 1;      /* array [0,1] */

    /* Build arrays of increasing length up to L.  The loop
       runs L-2 times, adding one new element each iteration. */
    for (idx_t len = 2; len < L; ++len) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int p1 = 0; p1 < 9; ++p1) {
            for (int p2 = 0; p2 < 9; ++p2) {
                for (int m = 0; m < 9; ++m) {
                    seq_t cur = dp[p1][p2][m];
                    if (!cur) continue;

                    /* Allowed new values: all existing values 0..m,
                       plus possibly a new value m+1 if m < 8. */
                    for (int c = 0; c <= m; ++c) {
                        /* aba restriction: if previous two are distinct,
                           we cannot repeat the first one. */
                        if (p1 != p2 && c == p1) continue;
                        int new_m = m;                 /* no new value used */
                        dp_next[p2][c][new_m] += cur;
                    }
                    if (m < 8) {
                        int c = m + 1;                 /* new value */
                        if (!(p1 != p2 && c == p1)) {
                            int new_m = c;             /* new max becomes c */
                            dp_next[p2][c][new_m] += cur;
                        }
                    }
                }
            }
        }
        /* Prepare for next iteration. */
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* Sum all final states to obtain the total number of arrays. */
    seq_t total = 0;
    for (int p1 = 0; p1 < 9; ++p1) {
        for (int p2 = 0; p2 < 9; ++p2) {
            for (int m = 0; m < 9; ++m) {
                total += dp[p1][p2][m];
            }
        }
    }
    return total;
}
