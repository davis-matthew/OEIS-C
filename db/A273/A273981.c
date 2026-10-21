/*
 * A273981  Erroneous version of A005897.
 * https://oeis.org/A273981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 6*n^2 - 12*n + 8 for n>2; a(1)=1, a(2)=4
 * Range:     i8 n <= 5, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    seq_t nn = (seq_t)n;
    return (seq_t)6 * nn * nn - (seq_t)12 * nn + (seq_t)8;
}
