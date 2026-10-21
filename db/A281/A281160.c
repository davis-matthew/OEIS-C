/*
 * A281160  Number of nX3 0..1 arrays with no element equal to more than three
 *          of its king-move neighbors, with the exception of exactly two
 *          elements, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 0/1 arrays with exactly two violations of
 *            the king‑move neighbour rule, counting only arrays whose first
 *            element (top‑left) is 0. The DP counts each array exactly once
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 75,
 *            big any n
 * Helpers:   violations_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/violations_row.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe */

    /* Trivial one‑row case */
    if (n == 1) {
        seq_t cnt = 0;
        for (uint8_t r = 0; r < 8; ++r) {
            if ((r & 1) == 0) {          /* first element must be 0 */
                int v = violations_row(0, r, 0, false, false);
                if (v == 2) ++cnt;
            }
        }
        return cnt;
    }

    /* DP tables: dp[prev_row][curr_row][violations_so_far]   */
    static seq_t dp[8][8][3];
    static seq_t ndp[8][8][3];

    /* Zero tables */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 3; ++k) dp[i][j][k] = 0;

    /* Initial state: process the first two rows.
       Count violations of the first row (has no upper neighbor).  */
    for (uint8_t r0 = 0; r0 < 8; ++r0) {
        if ((r0 & 1) != 0) continue;          /* enforce top‑left is 0 */
        for (uint8_t r1 = 0; r1 < 8; ++r1) {
            int v0 = violations_row(0, r0, r1, false, true);
            if (v0 <= 2) dp[r0][r1][v0] += 1;
        }
    }

    /* Process subsequent rows (indices 2 … n‑1) */
    for (idx_t row = 2; row <= n - 1; ++row) {
        /* Zero next table */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k) ndp[i][j][k] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                for (int v = 0; v < 3; ++v) {
                    seq_t ways = dp[prev][curr][v];
                    if (!ways) continue;
                    for (uint8_t next = 0; next < 8; ++next) {
                        int v_cur = violations_row(prev, curr, next, true, true);
                        int v_new = v + v_cur;
                        if (v_new <= 2) ndp[curr][next][v_new] += ways;
                    }
                }
            }
        }
        /* move ndp into dp for the next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = ndp[i][j][k];
    }

    /* Final row: compute its violations (no lower neighbor) */
    seq_t answer = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            int v_last = violations_row(prev, curr, 0, true, false);
            for (int v = 0; v < 3; ++v) {
                seq_t ways = dp[prev][curr][v];
                if (!ways) continue;
                if (v + v_last == 2) answer += ways;
            }
        }
    }
    return answer;
}
