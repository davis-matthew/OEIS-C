/*
 * A349682  a(n) = A000292(6*n + 1) where A000292 are the tetrahedral numbers.
 * https://oeis.org/A349682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + 2*n)*(1 + 3*n)*(1 + 6*n)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (1 + 2*nn) * (1 + 3*nn) * (1 + 6*nn);
    return term;
}
