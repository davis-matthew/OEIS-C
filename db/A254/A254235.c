/*
 * A254235  Number of (n+2)X(1+2) 0..1 arrays with every 3X3 subblock diagonal
 *          sum plus antidiagonal minimum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A254235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 binary arrays where, for every 3×3
 *            subblock, the value f = (sum of main diagonal cells) + (minimum
 *            of anti‑diagonal cells) is nondecreasing as the subblock moves
 *            down. Computation via dynamic programming over consecutive rows
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 34, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Number of rows in the array is n+2 */
    idx_t rows = n + 2;

    /* Precompute f for every triple of rows (each 3‑bit pattern). */
    static int f_table[8][8][8];
    static int f_initialized = 0;
    if (!f_initialized) {
        for (int r0 = 0; r0 < 8; ++r0) {
            for (int r1 = 0; r1 < 8; ++r1) {
                for (int r2 = 0; r2 < 8; ++r2) {
                    int diagSum =
                        ((r0 >> 0) & 1) + ((r1 >> 1) & 1) + ((r2 >> 2) & 1);
                    int antiMin =
                        ((r0 >> 2) & 1) & ((r1 >> 1) & 1) & ((r2 >> 0) & 1);
                    f_table[r0][r1][r2] = diagSum + antiMin;   /* value 0..4 */
                }
            }
        }
        f_initialized = 1;
    }

    /* DP state: last two rows (indices 0..7) and last f value (0..4). */
    seq_t dp[8][8][5] = {0}, next[8][8][5] = {0};

    /* Initial state: first three rows. */
    for (int r0 = 0; r0 < 8; ++r0) {
        for (int r1 = 0; r1 < 8; ++r1) {
            for (int r2 = 0; r2 < 8; ++r2) {
                int f0 = f_table[r0][r1][r2];
                dp[r1][r2][f0] += (seq_t)1;
            }
        }
    }

    /* Add rows from index 3 up to rows-1; each addition creates a new f. */
    idx_t add_rows = rows - 3;    /* number of rows to add after the first 3 */
    for (idx_t step = 0; step < add_rows; ++step) {
        /* clear next array */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int f = 0; f < 5; ++f)
                    next[i][j][f] = (seq_t)0;

        /* transition */
        for (int r1 = 0; r1 < 8; ++r1) {
            for (int r2 = 0; r2 < 8; ++r2) {
                for (int f = 0; f < 5; ++f) {
                    seq_t cur = dp[r1][r2][f];
                    if (!cur) continue;
                    for (int r3 = 0; r3 < 8; ++r3) {
                        int f_new = f_table[r1][r2][r3];
                        if (f_new >= f) {
                            next[r2][r3][f_new] += cur;
                        }
                    }
                }
            }
        }

        /* copy next into dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int f = 0; f < 5; ++f)
                    dp[i][j][f] = next[i][j][f];
    }

    /* Sum over all final states to get the answer. */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int f = 0; f < 5; ++f)
                ans += dp[i][j][f];

    return ans;
}
