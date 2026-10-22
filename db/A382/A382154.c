/*
 * A382154  a(0) = 1; thereafter a(n) = 2*n if n even or 4*n if n odd.
 * https://oeis.org/A382154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, a(n) = 2*n if n even, 4*n if n odd
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;
    return ((n & 1) == 0) ? (nn * (seq_t)2) : (nn * (seq_t)4);
}
