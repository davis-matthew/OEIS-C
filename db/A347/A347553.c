/*
 * A347553  Number of minimum dominating sets in the n-cycle complement graph.
 * https://oeis.org/A347553
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-3)/2 for n > 4; a(3)=1; a(4)=4
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)4;
    seq_t nn = (seq_t)n;
    return nn * (nn - (seq_t)3) / (seq_t)2;
}
