/*
 * A259058  Numbers that are representable in at least two ways as sums of
 *          four distinct nonvanishing squares.
 * https://oeis.org/A259058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 72*n + 454
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 4 * nn * nn + 72 * nn + 454;
    return res;
}
