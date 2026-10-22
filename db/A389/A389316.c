/*
 * A389316  Lower (2/3)-midsequence of square numbers and triangular numbers;
 *          see Comments.
 * https://oeis.org/A389316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n^2 + n)/3)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* cast index to seq_t */
    return nn * nn + nn / 3;       /* n^2 + floor(n/3) */
}
