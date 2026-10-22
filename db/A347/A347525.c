/*
 * A347525  Number of minimum dominating sets in the n-Andrásfai graph.
 * https://oeis.org/A347525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(3*n-1) for n > 3, with a(1)=2, a(2)=5, a(3)=24
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)24;
    seq_t nn = (seq_t)n;
    return (seq_t)2 * ((seq_t)3 * nn - (seq_t)1);
}
