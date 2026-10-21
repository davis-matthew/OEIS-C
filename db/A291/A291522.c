/*
 * A291522  The arithmetic function uhat(n,4,7).
 * https://oeis.org/A291522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -20 for n <= 10; a(n) = -2*n otherwise
 * Range:     i8 n <= 64, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 10)
        return (seq_t)-20;
    return -(seq_t)(2 * n);
}
