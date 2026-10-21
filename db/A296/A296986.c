/*
 * A296986  Number of nX4 0..1 arrays with each 1 adjacent to 3, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over rows using a two-row state. */
/* The array is “n×4”. Each row is a 4‑bit pattern (0–15).  */
/* For every 1‑cell its king‑neighbour count must be 3, 4 or 5.  */
/* We keep the last two rows (prev, curr) in the DP state.  */
/* After reading a new row “next”, we can check all 1‑cells of the current  */
/* row, because all its neighbours are then known.  */
/* Finally we add a phantom zero row beneath the last row to check its  */
/* constraints.  */

#include <stdbool.h>

/* Main counting function */
seq_t a(idx_t n)
{
    /* Pre‑compute validity table and next‑state adjacency lists. */
    static bool valid[16][16][16];
    static unsigned char nextList[16][16][16];
    static unsigned char nextCnt[16][16];
    static int initialized = 0;

    if (!initialized) {
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                for (int next = 0; next < 16; ++next) {
                    /* Check validity of curr row given neighbours. */
                    bool ok = true;
                    for (int col = 0; col < 4; ++col) {
                        if (!(curr & (1 << col))) continue;
                        int cnt = 0;
                        for (int dr = -1; dr <= 1; ++dr) {
                            for (int dc = -1; dc <= 1; ++dc) {
                                if (dr == 0 && dc == 0) continue;
                                int ncol = col + dc;
                                if (ncol < 0 || ncol >= 4) continue;
                                int rowMask;
                                if (dr == -1)
                                    rowMask = prev;
                                else if (dr == 0)
                                    rowMask = curr;
                                else /* dr == 1 */
                                    rowMask = next;
                                if (rowMask & (1 << ncol))
                                    ++cnt;
                            }
                        }
                        if (!(cnt == 3 || cnt == 4 || cnt == 5)) {
                            ok = false;
                            break;
                        }
                    }
                    valid[prev][curr][next] = ok;
                    if (ok) {
                        nextList[prev][curr][nextCnt[prev][curr]++] = (unsigned char)next;
                    }
                }
            }
        }
        initialized = 1;
    }

    /* DP tables for current and next step. */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];
    int i, j;
    for (i = 0; i < 16; ++i)
        for (j = 0; j < 16; ++j)
            dp[i][j] = 0, ndp[i][j] = 0;

    /* Initial state: prev row is all zeroes. */
    for (j = 0; j < 16; ++j)
        dp[0][j] = 1;

    /* Step through rows 1 .. n-1.  */
    for (idx_t step = 1; step < n; ++step) {
        for (i = 0; i < 16; ++i) {
            for (j = 0; j < 16; ++j) {
                seq_t cur = dp[i][j];
                if (cur == 0) continue;           /* skip zero entries */
                unsigned char *lst = nextList[i][j];
                int cnt = nextCnt[i][j];
                for (int k = 0; k < cnt; ++k) {
                    int next = (int)lst[k];
                    ndp[j][next] += cur;
                }
            }
        }
        /* Copy ndp -> dp and clear ndp. */
        for (i = 0; i < 16; ++i)
            for (j = 0; j < 16; ++j) {
                dp[i][j] = ndp[i][j];
                ndp[i][j] = 0;
            }
    }

    /* Final verification: add a zero row below the last row. */
    seq_t result = 0;
    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            if (valid[i][j][0]) {
                result += dp[i][j];
            }
        }
    }
    return result;
}
