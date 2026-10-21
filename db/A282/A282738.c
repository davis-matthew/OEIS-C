/*
 * A282738  First differences of A282737.
 * https://oeis.org/A282738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2; a(n)=1 if n>1 even; a(n)=(n+5)/2 if n>1 odd
 * Range:     u8 n <= 290, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if ((n & 1) == 0) return (seq_t)1;
    return ((seq_t)n + 5) / 2;
}
