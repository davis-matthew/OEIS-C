/*
 * A377018  a(n) is the number of paths of a knight on square a1 to reach a
 *          position outside an 8 X 8 chessboard after n steps.
 * https://oeis.org/A377018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of knight paths of length n from a1 that leave an
 *            8×8 board. a(1) = 6, a(2) = 4, … (OEIS A377018)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const int moves[8][2] = {
    { 2,  1}, { 1,  2}, {-1,  2}, {-2,  1},
    {-2, -1}, {-1, -2}, { 1, -2}, { 2, -1}
};

seq_t a(idx_t n)
{
    /* Offset: the sequence starts at n = 1.  For n < 1 the
       function is not defined, but the caller will not ask for such
       values. */
    if (n < 1) return 0;

    /* DP arrays holding the number of paths ending on each square
       after the current step.  64 = 8 × 8 board. */
    seq_t dp_prev[64];
    seq_t dp_curr[64];

    /* Pointers that are swapped each iteration.  They avoid assigning
       to the array objects themselves. */
    seq_t *prev = dp_prev;
    seq_t *curr = dp_curr;

    /* Initialise: start at a1, which is square 0. */
    for (size_t i = 0; i < 64; ++i) prev[i] = 0;
    prev[0] = 1;

    seq_t result = 0;

    for (idx_t step = 1; step <= n; ++step) {
        /* Clear current array. */
        for (size_t i = 0; i < 64; ++i) curr[i] = 0;

        seq_t exits_this_step = 0;

        /* For every square, move the knight. */
        for (size_t idx = 0; idx < 64; ++idx) {
            seq_t val = prev[idx];
            if (!val) continue;

            int r = (int)(idx / 8);
            int c = (int)(idx % 8);

            for (int m = 0; m < 8; ++m) {
                int nr = r + moves[m][0];
                int nc = c + moves[m][1];

                if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) {
                    exits_this_step += val;           /* exit move */
                } else {
                    size_t dest = (size_t)nr * 8 + (size_t)nc;
                    curr[dest] += val;                /* stay on board */
                }
            }
        }

        if (step == n) result = exits_this_step;

        /* Swap the two arrays for the next step. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    return result;
}
