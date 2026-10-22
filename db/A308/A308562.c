/*
 * A308562  Squares visited a knight moving on a board numbered by Hilbert's
 *          space-filling curve and moving to the lowest-numbered available
 *          unvisited square at each step.
 * https://oeis.org/A308562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A308562 as defined by the rule:
 *   Knight starts at the corner square of an 8×8 board numbered
 *   by the standard Hilbert space‑filling curve (indices 1…64).
 *   At each step the knight moves to the lowest‑numbered
 *   still unvisited reachable square.  The sequence is the
 *   visited square numbers.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define BOARD 8   /* Board side length (2^3) */

static inline seq_t hilbert_index(int x, int y)
{
    /* Standard 2‑D Hilbert curve index (0‑based). */
    seq_t d = 0;
    for (int s = BOARD >> 1; s > 0; s >>= 1) {
        int rx = (x & s) ? 1 : 0;
        int ry = (y & s) ? 1 : 0;
        d += (seq_t)(s * s) * ((3 * rx) ^ ry);
        if (!ry) {
            if (rx) {
                x = BOARD - 1 - x;
                y = BOARD - 1 - y;
            }
            /* Swap x and y */
            int t = x; x = y; y = t;
        }
    }
    return d;   /* 0‑based index */
}

seq_t a(idx_t n)
{
    static bool visited[BOARD * BOARD] = {0};
    static seq_t *seq  = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static int curx = 0, cury = 0;

    if (n == 0) return 0; /* Undefined for n<1 */

    /* Allocate/extend sequence array on first use */
    if (seq == NULL) {
        seq_cap = 32;
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    }

    /* Generate terms until we have at least n terms or the knight
     * can no longer move. */
    while (seq_len < (size_t)n) {
        /* Record current square */
        seq[seq_len] = (seq_t)(hilbert_index(curx, cury) + 1);
        seq_len++;
        if (seq_len >= seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        visited[curx + cury * BOARD] = true;

        /* Find the lowest‑numbered reachable unvisited square */
        static const int dx[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
        static const int dy[8] = { 2, 1, -1, -2, -2, -1,  1,  2 };
        bool found = false;
        seq_t best_val = 0;
        int best_x = 0, best_y = 0;

        for (int k = 0; k < 8; ++k) {
            int nx = curx + dx[k];
            int ny = cury + dy[k];
            if (nx < 0 || nx >= BOARD || ny < 0 || ny >= BOARD) continue;
            int idx = nx + ny * BOARD;
            if (visited[idx]) continue;
            seq_t val = (seq_t)(hilbert_index(nx, ny) + 1);
            if (!found || val < best_val) {
                found = true;
                best_val = val;
                best_x = nx;
                best_y = ny;
            }
        }

        if (!found) /* Knight is trapped */
            break;

        curx = best_x;
        cury = best_y;
    }

    /* If the requested index exceeds the generated terms (should
     * only happen for n > 23), return the last valid term. */
    if (n <= seq_len)
        return seq[n - 1];
    return seq_len > 0 ? seq[seq_len - 1] : 0;
}
