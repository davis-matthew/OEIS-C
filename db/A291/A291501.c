/*
 * A291501  The arithmetic function uhat(n,1,7).
 * https://oeis.org/A291501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -41 if n ≤ 8, otherwise -5*n (OEIS A291501)
 * Range:     i8 n <= 25, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 8)
        return (seq_t)-41;
    else
        return -((seq_t)5 * (seq_t)n);
}
