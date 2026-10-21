/*
 * A260810  a(n) = n^2*(3*n^2 - 1)/2.
 * https://oeis.org/A260810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(3*n^2 - 1)/2
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 230, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * n;         /* n^2 */
    seq_t t  = 3 * nn - 1;           /* 3*n^2 - 1 */
    return nn * t / 2;               /* (n^2*(3*n^2 - 1))/2 */
}
