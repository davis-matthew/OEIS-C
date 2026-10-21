/*
 * A295201  Number of n X 4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 2 or 4 1s.
 * https://oeis.org/A295201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295201: Number of n×4 binary arrays where every 1 is horizontally
   or vertically adjacent to exactly 2 or 4 other 1s.
   The function uses a dynamic‑programming approach over the rows.
   It runs in O(n·16·16·16) ≈ O(n) time and uses only constant extra
   memory. */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined – return 0. */
    if (n < 1) return (seq_t)0;

    /* ------------------------------------------ */
    /* Precompute triple‑row validity table once. */
    /* ------------------------------------------ */
    static unsigned char valid[16][16][16];
    static int valid_done = 0;
    if (!valid_done) {
        for (int up = 0; up < 16; ++up) {
            for (int mid = 0; mid < 16; ++mid) {
                for (int down = 0; down < 16; ++down) {
                    unsigned char ok = 1;
                    for (int j = 0; j < 4; ++j) {
                        if ((mid >> j) & 1) {
                            int cnt = 0;
                            /* left neighbour */
                            if (j > 0 && ((mid >> (j - 1)) & 1)) cnt++;
                            /* right neighbour */
                            if (j < 3 && ((mid >> (j + 1)) & 1)) cnt++;
                            /* vertical neighbours (possibly absent) */
                            if ((up >> j) & 1)   cnt++;
                            if ((down >> j) & 1) cnt++;
                            if (cnt != 2 && cnt != 4) {
                                ok = 0;
                                break;
                            }
                        }
                    }
                    valid[up][mid][down] = ok;
                }
            }
        }
        valid_done = 1;
    }

    /* ------------------------------------------ */
    /* Dynamic programming over rows.             */
    /* dp[prev][curr] = number of ways to reach a
       prefix ending with rows (prev, curr)          */
    /* ------------------------------------------ */
    seq_t dp[16][16];
    seq_t ndp[16][16];

    /* initialise dp to zero */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = (seq_t)0;

    /* first row – no row above (treated as zero) */
    for (int row = 0; row < 16; ++row) {
        dp[0][row] = (seq_t)1;
    }

    /* process remaining rows */
    for (idx_t r = 1; r < n; ++r) {
        /* zero ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = (seq_t)0;

        /* transition */
        for (int up = 0; up < 16; ++up) {
            for (int mid = 0; mid < 16; ++mid) {
                seq_t curcnt = dp[up][mid];
                if (curcnt == (seq_t)0) continue;
                for (int down = 0; down < 16; ++down) {
                    if (valid[up][mid][down]) {
                        ndp[mid][down] += curcnt;
                    }
                }
            }
        }

        /* copy ndp back to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final validation – bottom row is treated as zero */
    seq_t result = (seq_t)0;
    for (int up = 0; up < 16; ++up) {
        for (int mid = 0; mid < 16; ++mid) {
            if (valid[up][mid][0]) {
                result += dp[up][mid];
            }
        }
    }

    return result;
}
