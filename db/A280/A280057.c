/*
 * A280057  a(n) = (-1)^n * 4*n/3 if n = 3*k and n!=0, otherwise a(n) = (-1)^n
 *          * n except a(0) = 1.
 * https://oeis.org/A280057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * 4*n/3 if n % 3 == 0 && n != 0, otherwise a(n) =
 *            (-1)^n * n, with a(0) = 1
 * Range:     i8 n <= 32, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t sign = ((n & 1) == 0) ? 1 : -1;
    if (n % 3 == 0) {
        seq_t val = (seq_t)n * 4;
        val = val / 3;            /* exact division since n % 3 == 0 */
        return sign * val;
    } else {
        return sign * (seq_t)n;
    }
}
