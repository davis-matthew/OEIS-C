/*
 * A389317  Upper (2/3)-midsequence of square numbers and triangular numbers;
 *          see Comments.
 * https://oeis.org/A389317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((3*n^2 + n)/3)
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t prod = nn * (((seq_t)3 * nn) + 1);
    return (prod + 2) / 3;
}
