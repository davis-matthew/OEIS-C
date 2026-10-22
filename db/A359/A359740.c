/*
 * A359740  Maximal number of moves needed by a knight to reach every cell
 *          from a fixed position on an n X n X n chessboard, or -1 if it is
 *          not possible to reach every square.
 * https://oeis.org/A359740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 for n=1; -1 for n=2,3; n for n≥4
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n) {
        case 1: return 0;
        case 2: case 3: return (seq_t)-1;
        default: return (seq_t)n;
    }
}
