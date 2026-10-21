/*
 * A278824  Knight moves in chess: possible difference between origin and
 *          destination square when the squares are numbered sequentially row
 *          by row.
 * https://oeis.org/A278824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, easy, full
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Knight move differences: the n‑th smallest value of Y‑X for a knight move on an 8×8 chessboard. */

seq_t a(idx_t n)
{
    static seq_t diffs[8];
    static int init = 0;
    if (!init) {
        /* Relative knight moves. */
        const int8_t dx[8] = { 1, -1,  1, -1,  2, -2,  2, -2 };
        const int8_t dy[8] = { 2,  2, -2, -2,  1,  1, -1, -1 };
        /* Compute the differences for each move. */
        for (idx_t i = 0; i < 8; ++i) {
            diffs[i] = (seq_t)(8 * (seq_t)dy[i] + (seq_t)dx[i]);
        }
        /* Sort them ascending (bubble sort). */
        for (idx_t i = 0; i < 8; ++i) {
            for (idx_t j = i + 1; j < 8; ++j) {
                if (diffs[j] < diffs[i]) {
                    seq_t t = diffs[i];
                    diffs[i] = diffs[j];
                    diffs[j] = t;
                }
            }
        }
        init = 1;
    }
    if (n >= 1 && n <= 8) {
        return diffs[(size_t)(n - 1)];
    }
    /* For n > 8 the sequence is undefined; return 0. */
    return 0;
}
