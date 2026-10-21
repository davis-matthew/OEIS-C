/*
 * A289870  a(n) = n*(n + 1) for n odd, otherwise a(n) = (n - 1)*(n + 1).
 * https://oeis.org/A289870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n + 1)*(n - 1 + (n mod 2))
 * Range:     i8 n <= 10, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) != 0)
        return nn * (nn + 1);
    else
        return (nn - 1) * (nn + 1);
}
