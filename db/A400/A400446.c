/*
 * A400446  Number of 0-cells captured, Othello-style in all 8 directions,
 *          when the binary expansion of n is written in row n,
 *          right-justified beneath rows 1..n-1.
 * https://oeis.org/A400446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, new
 * Formula:   a(n) is computed by simulating the Othello capture process for
 *            the row n when the binary expansion of n is written
 *            right‑justified beneath all previous rows. No closed formula is
 *            known; the algorithm below constructs the board up to row n,
 *            scans each 1 in row n in all 8 directions, and counts eve...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline unsigned bitlen_idx(idx_t x)
{
    /* number of bits in a non‑zero idx_t value */
    unsigned b = 0;
    while (x) {
        ++b;
        x >>= 1;
    }
    return b;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    const unsigned bits_n = bitlen_idx(n);         /* width of the board */
    const size_t row_size = bits_n;                /* columns per row */
    const size_t board_cells = (size_t)(n + 1) * row_size; /* +1 for dummy row 0 */

    /* board holds 0, 1 and 2 (2 = non‑existent cell) */
    unsigned char *board = (unsigned char *)malloc(board_cells);
    unsigned char *capt   = (unsigned char *)malloc(board_cells);
    if (!board || !capt) return 0;                 /* allocation failure */

    memset(board, 2, board_cells);     /* initialise all to 2 */
    memset(capt, 0, board_cells);      /* no captured zero yet */

    /* fill rows 1..n‑1 */
    for (idx_t r = 1; r <= n - 1; ++r) {
        unsigned bits_r = bitlen_idx(r);
        unsigned shift  = bits_n - bits_r;          /* left padding for right‑justification */
        for (unsigned j = 0; j < bits_r; ++j) {
            unsigned bit = (r >> (bits_r - 1 - j)) & 1;
            board[(size_t)r * row_size + shift + j] = (unsigned char)bit;
        }
    }

    /* fill row n (the new row) */
    for (unsigned j = 0; j < bits_n; ++j) {
        unsigned bit = (n >> (bits_n - 1 - j)) & 1;
        board[(size_t)n * row_size + j] = (unsigned char)bit;
    }

    seq_t count = 0;

    /* 8 directions: N, NE, E, SE, S, SW, W, NW */
    static const int dr[8] = {-1, -1,  0, 1, 1,  1, 0, -1};
    static const int dc[8] = { 0,  1,  1, 1, 0, -1,-1, -1};

    /* store intermediate zero coordinates along a line */
    idx_t zero_stack[64];
    int zcnt;

    for (int col = 0; col < bits_n; ++col) {
        if (board[(size_t)n * row_size + col] != 1) continue; /* only start from a 1 */

        for (int d = 0; d < 8; ++d) {
            zcnt = 0;
            int r = (int)n + dr[d];
            int c = col + dc[d];
            while (r >= 1 && r <= (int)n && c >= 0 && c < (int)row_size) {
                unsigned char val = board[(size_t)r * row_size + c];
                if (val == 2) break;                 /* nonexistent cell */

                if (val == 0) {
                    zero_stack[zcnt++] = (idx_t)r * row_size + c;
                } else if (val == 1) {
                    /* old 1 found – capture all zeros in this line */
                    for (int t = 0; t < zcnt; ++t) {
                        idx_t idx = zero_stack[t];
                        if (!capt[idx]) {
                            capt[idx] = 1;
                            ++count;
                        }
                    }
                    break;
                }
                r += dr[d];
                c += dc[d];
            }
        }
    }

    free(board);
    free(capt);
    return count;
}
