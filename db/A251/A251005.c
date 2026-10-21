/*
 * A251005  Number of (n+1)X(3+1) 0..2 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than the absolute difference of its
 *          antidiagonal elements.
 * https://oeis.org/A251005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 matrices over {0,1,2} such that every
 *            2×2 subblock satisfies max(diagonal) ≥ |difference of
 *            anti‑diagonal|
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses a constant‑time DP over the 81 possible rows. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static tables of rows and admissible adjacencies,
       initialised on the first call. */
    static int     init   = 0;
    static uint8_t row[81][4];
    static unsigned char adj[81][81];

    if (!init) {
        /* Generate all 3^4 = 81 rows as base‑3 digits. */
        for (idx_t id = 0; id < 81; ++id) {
            idx_t x = id;
            for (int c = 0; c < 4; ++c) {
                row[id][c] = (uint8_t)(x % 3);
                x /= 3;
            }
        }

        /* Pre‑compute adjacency matrix: adj[i][j] is true if rows i and j
           can be consecutive in a matrix. */
        for (idx_t i = 0; i < 81; ++i) {
            for (idx_t j = 0; j < 81; ++j) {
                int ok = 1;                      /* assume compatible   */
                for (int col = 0; col < 3; ++col) {
                    int a = row[i][col];
                    int b = row[i][col + 1];
                    int c = row[j][col];
                    int d = row[j][col + 1];
                    int mx   = a > d ? a : d;    /* maximum diagonal    */
                    int diff = b > c ? (b - c) : (c - b); /* |anti‑diag|   */
                    if (mx < diff) { ok = 0; break; }
                }
                adj[i][j] = (unsigned char)ok;
            }
        }
        init = 1;
    }

    /* DP over rows: cur[i] = number of ways to end with row i
       after the current number of rows.  next is temporary. */
    seq_t cur[81];
    seq_t next[81];
    for (int i = 0; i < 81; ++i) cur[i] = (seq_t)1;   /* one way for first row */

    for (idx_t step = 0; step < n; ++step) {         /* n steps → (n+1) rows */
        for (int i = 0; i < 81; ++i) next[i] = (seq_t)0;
        for (int i = 0; i < 81; ++i) {
            /* Skip rows that are not reachable to reduce work. */
            if (cur[i] == (seq_t)0) continue;
            for (int j = 0; j < 81; ++j) {
                if (adj[i][j]) {
                    next[j] += cur[i];
                }
            }
        }
        for (int i = 0; i < 81; ++i) cur[i] = next[i];
    }

    /* Total number of matrices is the sum over all ending rows. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 81; ++i) total += cur[i];
    return total;
}
