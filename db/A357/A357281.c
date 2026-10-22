/*
 * A357281  The numbers of a square spiral with 1 in the center, lying at
 *          integer points of the right branch of the parabola y=n^2.
 * https://oeis.org/A357281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^4 + 3*n^2 + n + 1
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t for exact arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;       /* n^2   */
    seq_t n4 = n2 * n2;       /* n^4   */
    return (seq_t)4 * n4 + (seq_t)3 * n2 + nn + (seq_t)1;
}
