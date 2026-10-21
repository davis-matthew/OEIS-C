/*
 * A251095  Number of (n+1)X(3+1) 0..1 arrays with no 2X2 subblock having its
 *          minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A251095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Number of (n+1)×4 binary matrices with no 2×2 subblock
 * having its minimum diagonal element less than its minimum antidiagonal
 * element.  Computation is via dynamic programming on a 16‑state
 * adjacency graph representing allowed consecutive rows.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* adjacency matrix of allowed transitions between 4‑bit rows */
    static uint8_t adj[16][16];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int ok = 1;
                /* check all three overlapping 2×2 subblocks */
                for (int k = 0; k < 3; ++k) {
                    int A = (i >> k) & 1;      /* top‑left */
                    int B = (i >> (k + 1)) & 1;/* top‑right */
                    int C = (j >> k) & 1;      /* bottom‑left */
                    int D = (j >> (k + 1)) & 1;/* bottom‑right */
                    if (B && C && !(A && D)) { /* forbidden pattern */
                        ok = 0; break;
                    }
                }
                adj[i][j] = ok;
            }
        }
        initialized = 1;
    }

    /* counts[i] = number of sequences ending at state i after current step */
    seq_t count[16], next[16];
    for (int i = 0; i < 16; ++i) {
        count[i] = (seq_t)1;        /* first row can be any of 16 states */
    }

    for (idx_t step = 1; step <= n; ++step) {
        for (int j = 0; j < 16; ++j) next[j] = 0;
        for (int i = 0; i < 16; ++i) if (count[i]) {
            for (int j = 0; j < 16; ++j) if (adj[i][j]) {
                next[j] = next[j] + count[i];
            }
        }
        for (int j = 0; j < 16; ++j) count[j] = next[j];
    }

    seq_t total = 0;
    for (int i = 0; i < 16; ++i) total = total + count[i];
    return total;
}
