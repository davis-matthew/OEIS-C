/*
 * A316458  Expansion of 60*x*(1 + 4*x + x^2) / (1 - x)^5.
 * https://oeis.org/A316458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^4 + 30*n^3 + 15*n^2
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 129, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    return 15 * n4 + 30 * n3 + 15 * n2;
}
