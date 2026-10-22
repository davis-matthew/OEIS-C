/*
 * A389318  Upper (1/4)-midsequence of square numbers and (-1)*(triangular
 *          numbers); see Comments.
 * https://oeis.org/A389318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((n^2 - n)/8)
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t x  = nn * (nn - 1);     /* n*(n-1) = n^2 - n */
    return (x + 7) / 8;           /* ceil division by 8 */
}
