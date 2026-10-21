/*
 * A291517  The arithmetic function uhat(n,3,8).
 * https://oeis.org/A291517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -39 for n <= 9, a(n) = -4*n for n >= 10
 * Range:     i8 n <= 32, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 9)
        return (seq_t)-39;
    else
        return (seq_t)-4 * (seq_t)n;
}
