/*
 * A340519  Smallest order of a non-abelian group with a center of order n.
 * https://oeis.org/A340519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6n if n is odd, 4n if n is even
 * Range:     u8 n <= 42, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    return nt * ((nt & 1) ? 6 : 4);
}
