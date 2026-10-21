/*
 * A297736  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A297736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays in which every 1 has *
 *            exactly 2 or 4 neighbors that are horizontally adjacent
 *            (left/right) * or diagonally (up‑left / down‑right) or
 *            antidiagonally (up‑right / down‑left). * * This implementation
 *            uses dynamic programming over column triples. * Only 16
 *            possible...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /*
     * Pre‑compute whether a triple (left, mid, right) satisfies the
     * condition that every 1 in the middle column has 2 or 4 allowed
     * neighbors (horizontal or diagonal/antidiagonal) among the three
     * adjacent columns.  Vertical neighbours are NOT counted.
     */
    static bool valid[16][16][16];
    static bool initialized = false;
    if (!initialized) {
        for (int left = 0; left < 16; ++left) {
            for (int mid = 0; mid < 16; ++mid) {
                for (int right = 0; right < 16; ++right) {
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        if (!((mid >> r) & 1)) continue;      /* no 1 in this cell */

                        int cnt = 0;
                        /* horizontal neighbours */
                        cnt += (left >> r) & 1;              /* left same row */
                        cnt += (right >> r) & 1;             /* right same row */

                        /* diagonal neighbours (up‑left, down‑left) */
                        if (r > 0)   cnt += (left >> (r-1)) & 1;   /* up-left */
                        if (r < 3)   cnt += (left >> (r+1)) & 1;   /* down-left */

                        /* antidiagonal neighbours (up‑right, down‑right) */
                        if (r > 0)   cnt += (right >> (r-1)) & 1;  /* up-right */
                        if (r < 3)   cnt += (right >> (r+1)) & 1;  /* down-right */

                        if (!(cnt == 2 || cnt == 4)) { ok = false; break; }
                    }
                    valid[left][mid][right] = ok;
                }
            }
        }
        initialized = true;
    }

    /* DP arrays */
    seq_t dp[16][16] = {0};
    seq_t nextdp[16][16] = {0};

    /* initialise for the first real column : left dummy = 0 */
    for (int p = 0; p < 16; ++p)
        dp[0][p] = 1;

    /* add columns 2 .. n */
    for (idx_t col = 2; col <= n; ++col) {
        /* clear nextdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                nextdp[i][j] = 0;

        /* transition */
        for (int left = 0; left < 16; ++left) {
            for (int mid = 0; mid < 16; ++mid) {
                seq_t ways = dp[left][mid];
                if (ways == 0) continue;
                for (int right = 0; right < 16; ++right) {
                    if (valid[left][mid][right]) {
                        nextdp[mid][right] += ways;
                    }
                }
            }
        }

        /* copy nextdp to dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* final column must satisfy condition with dummy right = 0 */
    seq_t result = 0;
    for (int left = 0; left < 16; ++left) {
        for (int mid = 0; mid < 16; ++mid) {
            if (valid[left][mid][0]) {
                result += dp[left][mid];
            }
        }
    }
    return result;
}
