/*
 * A389319  Lower (3/4)-midsequence of square numbers and negative triangular
 *          numbers; see Comments.
 * https://oeis.org/A389319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3/8)*(n^2 - n))
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nn = (seq_t)n;
    seq_t t = nn * (nn - 1);   /* n*(n-1) */
    return (t * 3) / 8;
}
