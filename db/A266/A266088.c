/*
 * A266088  Alternating sum of 12-gonal (or dodecagonal) numbers.
 * https://oeis.org/A266088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 + (-1)^n * (5*n^2 + n - 2)/2
 * Range:     i8 n <= 7, i16 n <= 114, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                          /* cast index to seq_t */
    seq_t inner = (5 * nn * nn + nn - 2) / 2;     /* (5*n^2 + n - 2)/2 */
    seq_t sign = (n & 1) ? -1 : 1;                /* (-1)^n */
    return 1 + sign * inner;                     /* final result */
}
