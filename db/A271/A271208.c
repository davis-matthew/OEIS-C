/*
 * A271208  a(n) = n^5 + n - 1.
 * https://oeis.org/A271208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^5 + n - 1
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t r  = nn * nn * nn * nn * nn;  /* n^5 */
    r += nn;                           /* + n */
    r -= (seq_t)1;                     /* - 1 */
    return r;
}
