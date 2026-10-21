/*
 * A258809  a(n) = n^8 - 1.
 * https://oeis.org/A258809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * (n+1) * (n^2 + 1) * (n^4 + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    return (nn - (seq_t)1) * (nn + (seq_t)1) * (n2 + (seq_t)1) * (n4 + (seq_t)1);
}
