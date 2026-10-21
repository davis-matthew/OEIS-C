/*
 * A280237  Period 8 sequence [0, 1, 0, 1, -1, 1, 0, 1, ...].
 * https://oeis.org/A280237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = [0, 1, 0, 1, -1, 1, 0, 1] repeated every 8 terms
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t periodic[8] = {0, 1, 0, 1, -1, 1, 0, 1};
    return periodic[n & 7];
}
