/*
 * A323178  a(n) = 1 + 100*n^2 for n >= 0.
 * https://oeis.org/A323178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 100*n^2
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)1 + (seq_t)100 * nn * nn;
}
