/*
 * A297690  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A297690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×4 binary arrays such that
 * each 1 has 0, 2 or 4 neighbouring 1s among
 * horizontally adjacent cells in the same row and
 * diagonally / antidiagonally adjacent cells in the rows immediately above and below.
 * (This is OEIS A297690.)
 * The algorithm uses dynamic programming over two consecutive rows.
 */

#include <stdbool.h>
#include <stddef.h>

static bool ok_table[16][16][16];
static bool ok_table_computed = false;

/* pre‑compute the validity table:
 * ok_table[a][b][c] == true  ⇔  every 1 in row b (current) has 0,2 or 4
 * neighbours among (row a –1, row b – same row, row c +1). */
static void precompute_ok(void)
{
    if (ok_table_computed) return;
    ok_table_computed = true;
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int c = 0; c < 16; ++c) {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    if (!(b & (1 << j))) continue; /* cell is 0 – no restriction */
                    int cnt = 0;
                    /* same row */
                    if (j > 0 && (b & (1 << (j - 1)))) ++cnt;
                    if (j < 3 && (b & (1 << (j + 1)))) ++cnt;
                    /* from row a (up) */
                    if (j > 0 && (a & (1 << (j - 1)))) ++cnt;
                    if (j < 3 && (a & (1 << (j + 1)))) ++cnt;
                    /* from row c (down) */
                    if (j > 0 && (c & (1 << (j - 1)))) ++cnt;
                    if (j < 3 && (c & (1 << (j + 1)))) ++cnt;
                    if (cnt != 0 && cnt != 2 && cnt != 4) ok = false;
                }
                ok_table[a][b][c] = ok;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    precompute_ok();

    /* Handle the trivial case n = 1 separately */
    if (n == 1) {
        seq_t total = 0;
        for (int b = 0; b < 16; ++b) {
            if (ok_table[0][b][0]) total += 1;
        }
        return total;
    }

    /* DP arrays: number of ways to reach a state (row_{k-2}, row_{k-1}) */
    seq_t dp[256];
    seq_t newdp[256];

    /* initialise for the first two rows */
    for (size_t i = 0; i < 256; ++i) dp[i] = 0;
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            if (ok_table[0][r0][r1]) {
                dp[(r0 << 4) | r1] = 1;
            }
        }
    }

    /* iterate over remaining rows (from 3rd to n-th) */
    for (idx_t step = 2; step < n; ++step) {
        for (size_t i = 0; i < 256; ++i) newdp[i] = 0;
        for (int prev2 = 0; prev2 < 16; ++prev2) {
            for (int prev1 = 0; prev1 < 16; ++prev1) {
                seq_t curcnt = dp[(prev2 << 4) | prev1];
                if (!curcnt) continue;
                for (int r2 = 0; r2 < 16; ++r2) {
                    if (ok_table[prev2][prev1][r2]) {
                        unsigned idx = (prev1 << 4) | r2;
                        newdp[idx] += curcnt;
                    }
                }
            }
        }
        for (size_t i = 0; i < 256; ++i) dp[i] = newdp[i];
    }

    /* sum over final states where the last row satisfies its constraint with no next row */
    seq_t total = 0;
    for (int prev2 = 0; prev2 < 16; ++prev2) {
        for (int prev1 = 0; prev1 < 16; ++prev1) {
            if (ok_table[prev2][prev1][0]) {
                total += dp[(prev2 << 4) | prev1];
            }
        }
    }
    return total;
}
