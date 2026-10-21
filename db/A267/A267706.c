/*
 * A267706  a(n) = 137*n^2 - 4043*n + 27277.
 * https://oeis.org/A267706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 137*n^2 - 4043*n + 27277
 * Range:     i8 none, i16 n <= 30, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)137 * nn * nn - (seq_t)4043 * nn + (seq_t)27277;
}
