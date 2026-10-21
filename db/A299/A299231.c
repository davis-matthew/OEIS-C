/*
 * A299231  Ranks of {2,3}-power towers that start with 2; see Comments.
 * https://oeis.org/A299231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2n-1 for all n except a(3)=4, a(4)=6, a(6)=10
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)6;
    if (n == 6) return (seq_t)10;
    return (seq_t)n * (seq_t)2 - (seq_t)1;
}
