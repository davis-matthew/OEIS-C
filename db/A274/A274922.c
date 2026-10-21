/*
 * A274922  a(n) = (-1)^n * n if n>0, a(0) = 1.
 * https://oeis.org/A274922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * n for n > 0, a(0) = 1
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t val = (seq_t)n;
    return (n & 1) ? -val : val;
}
