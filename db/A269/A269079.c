/*
 * A269079  Number of 5Xn binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 5×n binary arrays with at most one pair of adjacent 1s
 *            (adjacent horizontally or diagonally)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* DP over columns.
   State: mask 5‑bit (column bits), flag usedEdge (0/1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Pre‑compute number of edges between any two column masks. */
    static bool initialized = false;
    static unsigned char edgeCnt[32][32];
    if (!initialized) {
        for (int p = 0; p < 32; ++p) {
            for (int c = 0; c < 32; ++c) {
                unsigned e = 0;
                /* horizontal edges */
                for (int r = 0; r < 5; ++r)
                    if ((p >> r & 1) && (c >> r & 1)) ++e;
                /* diagonal edges (p→c) */
                for (int r = 0; r < 4; ++r)
                    if ((p >> r & 1) && (c >> (r + 1) & 1)) ++e;
                /* anti‑diagonal edges (p→c) */
                for (int r = 1; r < 5; ++r)
                    if ((p >> r & 1) && (c >> (r - 1) & 1)) ++e;
                edgeCnt[p][c] = (unsigned char)e;
            }
        }
        initialized = true;
    }

    /* dp[mask][used] – number of partial arrays ending with the given
       column mask and used flag. */
    seq_t dp[32][2];
    seq_t next[32][2];
    for (int i = 0; i < 32; ++i) {
        dp[i][0] = (seq_t)1;   /* first column, no edge used yet */
        dp[i][1] = (seq_t)0;
    }

    /* Process columns 2 to n. */
    for (idx_t col = 2; col <= n; ++col) {
        /* zero next array */
        for (int i = 0; i < 32; ++i) {
            next[i][0] = (seq_t)0;
            next[i][1] = (seq_t)0;
        }
        for (int pm = 0; pm < 32; ++pm) {
            for (int used = 0; used < 2; ++used) {
                seq_t cur = dp[pm][used];
                if (!cur) continue;
                for (int cm = 0; cm < 32; ++cm) {
                    unsigned e = edgeCnt[pm][cm];
                    if (used) {
                        /* Edge already used. Only allowed if no new edges. */
                        if (e == 0)
                            next[cm][1] += cur;
                    } else {
                        if (e == 0)
                            next[cm][0] += cur;
                        else if (e == 1)
                            next[cm][1] += cur;
                    }
                }
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 32; ++i) {
            dp[i][0] = next[i][0];
            dp[i][1] = next[i][1];
        }
    }

    /* Sum over all final states. */
    seq_t result = (seq_t)0;
    for (int m = 0; m < 32; ++m) {
        result += dp[m][0];
        result += dp[m][1];
    }
    return result;
}
