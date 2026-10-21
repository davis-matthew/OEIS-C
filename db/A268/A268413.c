/*
 * A268413  a(n) = Sum_{k = 0..n} (-1)^k*14^k.
 * https://oeis.org/A268413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - 14*a(n-1), a(0) = 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 16, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)1;        /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        cur = (seq_t)1 - (seq_t)14 * cur;
    }
    return cur;
}
