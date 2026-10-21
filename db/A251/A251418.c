/*
 * A251418  Floor((n^2+7n-23)/14).
 * https://oeis.org/A251418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor((n^2+7n-23)/14)
 * Range:     i8 n <= 9, i16 n <= 192, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t numerator = nn * nn + 7 * nn - 23;
    if (numerator >= 0) {
        return numerator / 14;
    } else {
        /* For negative numerators, use (numerator-13)/14 to obtain floor division */
        return (numerator - 13) / 14;
    }
}
