/*
 * A259059  One half of numbers representable in at least two different ways
 *          as sums of four distinct nonvanishing squares. See A259058 for
 *          these numbers and their representations.
 * https://oeis.org/A259059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 36*n + 227
 * Range:     u8 n <= 0, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * x * 2 + x * 36 + 227;
}
