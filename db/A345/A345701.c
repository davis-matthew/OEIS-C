/*
 * A345701  a(n) = 3*n^3 - 1.
 * https://oeis.org/A345701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n^3 - 1
 * Range:     i8 n <= 3, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;   /* n^2 */
    res *= nn;             /* n^3 */
    res *= (seq_t)3;       /* 3*n^3 */
    res -= (seq_t)1;       /* 3*n^3 - 1 */
    return res;
}
