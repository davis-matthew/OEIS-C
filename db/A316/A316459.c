/*
 * A316459  Expansion of 30*x*(1 + x) / (1 - x)^4.
 * https://oeis.org/A316459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^3 + 15*n^2 + 5*n
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 753, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 10*nn*nn*nn + 15*nn*nn + 5*nn;
}
