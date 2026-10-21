/*
 * A251060  Number of (n+1)X(3+1) 0..2 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+1)×4 0..2 arrays with no 2×2 subblock having
   x11‑x00 less than x10‑x01 (OEIS A251060).
   Dynamic programming over row transitions – at each step
   a row is followed by every allowed row; only additions are
   required so 64‑bit builds never overflow for values that fit
   into 64 bits.                           */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 81 possible rows of length 4 over {0,1,2} */
    static bool built = false;
    static int   edge[81][81];
    static unsigned char edge_len[81];

    if (!built) {
        /* precompute digits of each row in base‑3 */
        int dig[81][4];
        for (int s = 0; s < 81; ++s) {
            int t = s;
            for (int k = 3; k >= 0; --k) {
                dig[s][k] = t % 3;
                t /= 3;
            }
        }
        /* build adjacency list of allowed transitions */
        for (int r1 = 0; r1 < 81; ++r1) {
            for (int r2 = 0; r2 < 81; ++r2) {
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    int x00 = dig[r1][j];
                    int x01 = dig[r1][j + 1];
                    int x10 = dig[r2][j];
                    int x11 = dig[r2][j + 1];
                    int d1  = x11 - x00;
                    int d2  = x10 - x01;
                    if (d1 < d2) { ok = false; break; }
                }
                if (ok)
                    edge[r1][edge_len[r1]++] = r2;
            }
        }
        built = true;
    }

    /* dynamic programming over n transitions (n steps) */
    seq_t cur[81];
    seq_t next[81];

    for (int i = 0; i < 81; ++i) cur[i] = (seq_t)1;   /* one way to start at any row */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 81; ++i) next[i] = 0;

        for (int i = 0; i < 81; ++i) {
            seq_t c = cur[i];
            if (c == 0) continue;
            for (int e = 0; e < edge_len[i]; ++e)
                next[edge[i][e]] += c;
        }
        for (int i = 0; i < 81; ++i) cur[i] = next[i];
    }

    /* sum of all ways of ending in any row */
    seq_t total = 0;
    for (int i = 0; i < 81; ++i) total += cur[i];
    return total;
}
