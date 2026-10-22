/*
 * A305071  a(n) = 972*n^2 - 270*n (n>=1).
 * https://oeis.org/A305071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 972*n^2 - 270*n
 * Range:     u8 none, u16 n <= 8, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, values for n<1 are irrelevant */
    seq_t nn = (seq_t)n;
    return (seq_t)972 * nn * nn - (seq_t)270 * nn;
}
