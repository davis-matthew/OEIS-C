/*
 * A246924  8 X 8 square array read by rows: T(i,j) = number of moves of a
 *          knight to reach the square (i,j) when starting from the square
 *          (1,2).
 * https://oeis.org/A246924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, full, tabf, fini
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A246924: minimum knight moves on an 8×8 board
   starting from square (1,2), read row‑by‑row. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t tbl[64];

    /* one‑time BFS to fill the distance table */
    if (!init) {
        init = true;
        int dist[64];
        for (int k = 0; k < 64; ++k) dist[k] = -1;

        /* start position (1,2) → index 1 */
        dist[1] = 0;
        int q[64], head = 0, tail = 0;
        q[tail++] = 1;

        const int di[8] = { 2,  2, -2, -2,  1,  1, -1, -1 };
        const int dj[8] = { 1, -1,  1, -1,  2, -2,  2, -2 };

        while (head < tail) {
            int cur = q[head++];
            int r = cur / 8 + 1;      /* 1‑based row */
            int c = cur % 8 + 1;      /* 1‑based column */
            int d = dist[cur];

            for (int k = 0; k < 8; ++k) {
                int nr = r + di[k];
                int nc = c + dj[k];
                if (nr >= 1 && nr <= 8 && nc >= 1 && nc <= 8) {
                    int idx = (nr - 1) * 8 + (nc - 1);
                    if (dist[idx] == -1) {
                        dist[idx] = d + 1;
                        q[tail++] = idx;
                    }
                }
            }
        }

        for (int k = 0; k < 64; ++k) tbl[k] = (seq_t)dist[k];
    }

    if (n == 0) return 0;          /* out of bounds below offset */
    idx_t idx = n - 1;
    if (idx >= 64) return 0;       /* board has only 64 positions */
    return tbl[(size_t)idx];
}
