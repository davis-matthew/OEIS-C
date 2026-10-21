/*
 * A250530  Number of (n+1)X(2+1) 0..3 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 * satisfying *
 *            max(x(i,j),x(i,j-1)) is non‑decreasing in i (j=1,2), *
 *            min(x(i,j),x(i‑1,j)) is non‑decreasing in j (i=1,n). * * The
 *            count is obtained by dynamic programming over the 64 * possible
 *            row patterns. For each pair of consecutive rows p,q...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define ROWS 64

seq_t a(idx_t n)
{
    /* adjacency matrix for transitions between rows (0..63) */
    static uint8_t adj[ROWS][ROWS];
    static bool prepared = false;

    if (!prepared) {
        for (int p = 0; p < ROWS; ++p) {
            int pcol[3] = {p % 4, (p / 4) % 4, (p / 16) % 4};
            for (int q = 0; q < ROWS; ++q) {
                int qcol[3] = {q % 4, (q / 4) % 4, (q / 16) % 4};
                bool ok = true;
                for (int j = 1; j <= 2; ++j) {
                    int pL = pcol[j - 1], pR = pcol[j];
                    int cL = qcol[j - 1], cR = qcol[j];
                    int max_cur = (cR > cL) ? cR : cL;
                    int max_prev = (pR > pL) ? pR : pL;
                    if (max_cur < max_prev) { ok = false; break; }
                    int min_cur = (cR < pR) ? cR : pR;
                    int min_prev = (cL < pL) ? cL : pL;
                    if (min_cur < min_prev) { ok = false; break; }
                }
                adj[p][q] = ok ? 1 : 0;
            }
        }
        prepared = true;
    }

    seq_t cur[ROWS];
    seq_t nxt[ROWS];
    for (int i = 0; i < ROWS; ++i) {
        cur[i] = 0;
        nxt[i] = 0;
        /* initial row can be any of the 64 patterns */
        cur[i] = 1;
    }

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < ROWS; ++i) nxt[i] = 0;
        for (int p = 0; p < ROWS; ++p) {
            if (cur[p] == 0) continue;
            seq_t val = cur[p];
            for (int q = 0; q < ROWS; ++q) {
                if (!adj[p][q]) continue;
                nxt[q] = nxt[q] + val;
            }
        }
        for (int i = 0; i < ROWS; ++i) cur[i] = nxt[i];
    }

    seq_t total = 0;
    for (int i = 0; i < ROWS; ++i) total = total + cur[i];
    return total;
}
