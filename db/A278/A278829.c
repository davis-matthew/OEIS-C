/*
 * A278829  Pawn's moves in chess: possible difference between origin and
 *          destination square when the squares are numbered sequentially row
 *          by row.
 * https://oeis.org/A278829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) are the sorted distinct differences Y‑X for all possible pawn moves
   on a standard 8×8 chessboard (row‑by‑row numbering). */
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t diffs[8];
    static idx_t cached = 0;
    if (cached == 0) {
        bool seen[33] = {0};   /* indexes 0..32 correspond to offsets -16..16 */
        for (idx_t X = 1; X <= 64; ++X) {
            idx_t row = (X - 1) / 8 + 1;     /* 1..8 */
            idx_t col = (X - 1) % 8 + 1;     /* 1..8 */

            /* White pawn moves */
            if (row < 8)     seen[24] = true;  /* +8   */
            if (row == 2)    seen[32] = true;  /* +16  */
            if (col > 1)     seen[23] = true;  /* +7   */
            if (col < 8)     seen[25] = true;  /* +9   */

            /* Black pawn moves */
            if (row > 1)     seen[8]  = true;  /* -8   */
            if (row == 7)    seen[0]  = true;  /* -16  */
            if (col > 1)     seen[7]  = true;  /* -9   */
            if (col < 8)     seen[9]  = true;  /* -7   */
        }

        idx_t pos = 0;
        for (int offset = -16; offset <= 16; ++offset) {
            if (seen[offset + 16]) {
                diffs[pos++] = (seq_t)offset;
            }
        }
        cached = pos;
    }

    if (n >= 1 && n <= cached) {
        return diffs[n - 1];
    }
    return (seq_t)0;   /* out of range – not used in tests */
}
