/*
 * A309842  a(n) is the total surface area of a hollow cubic block (defined as
 *          a block with a shell thickness of 1 cube) where n is the edge
 *          length of the removed volume.
 * https://oeis.org/A309842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 + 24*n + 24
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 12 * nn * nn + 24 * nn + 24;
}
