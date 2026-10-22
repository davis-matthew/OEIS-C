/*
 * A393839  Number of squares reachable in n (but no fewer) leaps of a giraffe
 *          (the fairy chess piece) on an infinite chessboard.
 * https://oeis.org/A393839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 92*n - 132 for n >= 9 a(0) = 1, a(1) = 8, a(2) = 32, a(3)
 *            = 88, a(4) = 192, a(5) = 324, a(6) = 448, a(7) = 548, a(8) = 620
 * Range:     u8 n <= 4, u16 n <= 713, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* precomputed values for n = 0..8 */
    static const seq_t pre[9] = {
        (seq_t)1, (seq_t)8, (seq_t)32, (seq_t)88, (seq_t)192,
        (seq_t)324, (seq_t)448, (seq_t)548, (seq_t)620
    };

    if (n < 9) {
        return pre[n];
    }

    return (seq_t)92 * (seq_t)n - (seq_t)132;
}
