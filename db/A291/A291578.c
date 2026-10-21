/*
 * A291578  The arithmetic function uhat(n,7,7).
 * https://oeis.org/A291578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n mod 7 ≠ 0; a(n) = –5 if n mod 7 = 0
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 7ull) == 0)
        return (seq_t)-5;
    else
        return (seq_t)1;
}
