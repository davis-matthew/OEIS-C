/*
 * A280569  a(n) = (-1)^n * 2 if n = 5*k and n!=0, otherwise a(n) = (-1)^n.
 * https://oeis.org/A280569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * 2 if n % 5 == 0 and n != 0, otherwise a(n) =
 *            (-1)^n
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* sign = (-1)^n */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;
    if ((n % 5) == 0) {
        return sign * 2;
    } else {
        return sign;
    }
}
