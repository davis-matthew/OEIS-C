/*
 * A316588  Squares visited by knight moves on a diagonally numbered Q1 board
 *          and moving to the lowest available unvisited square at each step.
 *          Starting square is labeled 1.
 * https://oeis.org/A316588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, look
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 2402, u32 n <= 2402, u64 n <= 2402,
 *            u128 n <= 2402, big any n
 * Helpers:   board_num, visited_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A316588: Greedy knight walk on a diagonally numbered first‑quadrant board.
   Starting position is square 1 (1,1).  At each step the knight moves to
   the lowest numbered unvisited square reachable by a standard knight move. */
/* Implements a(1) = 1, a(2) = 8, a(3) = 6, a(4) = 2, ... */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/board_num.h"
#include "../utils/visited_contains.h"

/* Dynamic storage for the sequence visited so far.                          */
static seq_t *seq      = NULL;      /* visited board numbers            */
static size_t seq_len  = 0;          /* number of visited squares        */
static size_t seq_cap  = 0;          /* current capacity of seq         */

/* Current knight coordinates (1‑based).                                    */
static seq_t _knight_x = 0;
static seq_t _knight_y = 0;

/* Ensure the sequence array has room for at least 'need' elements.          */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *newp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!newp)
        abort(); /* allocation failure */
    seq = newp;
    seq_cap = new_cap;
}

/* Convert (x,y) with x,y >= 1 to the board number according to the       */

/* Public function required by the build system.                              */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n = 0 */

    /* Initialise once when the first call is made.                          */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq[0]     = (seq_t)1;   /* board 1 */
        seq_len    = 1;
        _knight_x  = (seq_t)1;
        _knight_y  = (seq_t)1;
    }

    /* Knight move deltas (signed).  These are stored as signed ints so       */
    /* that negative steps do not wrap when added to the unsigned coordinates. */
    static const int dx[8] = { 1,  2,  2, 1, -1, -2, -2, -1 };
    static const int dy[8] = { 2,  1, -1,-2, -2, -1,  1,  2 };

    /* Grow the sequence until it contains at least n visited squares, or      */
    /* until the knight can no longer move.                                   */
    while (seq_len < (size_t)n) {
        seq_t best_num = 0;          /* candidate board number      */
        seq_t best_x   = 0, best_y = 0;
        bool found = false;

        for (int i = 0; i < 8; ++i) {
            /* Compute the candidate coordinates using signed arithmetic.    */
            int64_t cx = (int64_t)_knight_x + dx[i];
            int64_t cy = (int64_t)_knight_y + dy[i];
            if (cx < 1 || cy < 1)
                continue;           /* outside the first quadrant */

            seq_t ux = (seq_t)cx;
            seq_t uy = (seq_t)cy;

            seq_t cand = board_num(ux, uy);
            if (visited_contains(seq, seq_len, cand))
                continue;           /* already visited */

            if (!found || cand < best_num) {
                found = true;
                best_num = cand;
                best_x   = ux;
                best_y   = uy;
            }
        }

        if (!found)
            break;                  /* no further moves possible */

        /* Record the chosen square.                                        */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = best_num;
        _knight_x = best_x;
        _knight_y = best_y;
    }

    /* Return the nth visited board number (1‑based).                        */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];
    return (seq_t)0;                /* sequence finished before reaching n */
}
