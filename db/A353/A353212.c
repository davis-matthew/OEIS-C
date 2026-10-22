/*
 * A353212  Hadwiger number of the n-path complement graph.
 * https://oeis.org/A353212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n + 1)/4) for n >= 6 Initial values: a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=2, a(5)=3
 * Range:     u8 n <= 135, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)1;
    if (n == (idx_t)3) return (seq_t)2;
    if (n == (idx_t)4) return (seq_t)2;
    if (n == (idx_t)5) return (seq_t)3;

    seq_t x = (seq_t)n;
    x = x * (seq_t)3 + (seq_t)1;
    return x / (seq_t)4;
}
