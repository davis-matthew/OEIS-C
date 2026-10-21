/*
 * A261012  Sign(n) (with offset -1): a(n) = 1 if n>0, = -1 if n<0, = 0 if n =
 *          0.
 * https://oeis.org/A261012
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = -1 if n < 0, 0 if n == 0, 1 if n > 0
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)-1;
    if (n == 0) return (seq_t)0;
    return (seq_t)1;
}
