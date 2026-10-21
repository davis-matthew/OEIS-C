/*
 * A257352  Expansion of g.f. (1-2*x+51*x^2)/(1-x)^3.
 * https://oeis.org/A257352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 - 25*n + 1
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)25 * nn * nn - (seq_t)25 * nn + (seq_t)1;
}
