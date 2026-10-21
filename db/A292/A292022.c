/*
 * A292022  a(n) = 4*n*(n^2 + 2).
 * https://oeis.org/A292022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n^2 + 2)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = nn * nn + (seq_t)2;     /* n^2 + 2 */
    return (seq_t)4 * nn * temp;        /* 4 * n * (n^2 + 2) */
}
