/*
 * A331952  a(n) = (-7 + (-1)^(1+n) + 6*n^2) / 8.
 * https://oeis.org/A331952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-7 + (-1)^(1+n) + 6*n^2) / 8
 * Range:     i8 n <= 4, i16 n <= 202, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* parity: 0 for even n, 1 for odd n */
    idx_t parity = n & 1U;
    /* compute numerator = -7 + (-1)^(1+n) + 6*n^2 */
    seq_t numerator =
        (seq_t)-7
        + (seq_t)(parity ? 1 : -1)
        + (seq_t)6 * (seq_t)n * (seq_t)n;
    /* division by 8 is exact */
    return numerator / 8;
}
