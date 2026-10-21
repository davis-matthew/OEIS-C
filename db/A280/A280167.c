/*
 * A280167  a(2*n) = 3*n if n > 0, a(2*n + 1) = -(2*n + 1), a(0) = 1.
 * https://oeis.org/A280167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(n) = 3*(n/2) if n even > 0, a(n) = -n if n odd > 0
 * Range:     i8 n <= 85, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    if ((n & 1) != 0) {
        return -(seq_t)n;
    } else {
        return (seq_t)3 * ((seq_t)n >> 1);
    }
}
