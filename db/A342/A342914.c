/*
 * A342914  Number of grid points covered by a truncated triangle drawn on the
 *          hexagonal lattice with the short sides having length n and the
 *          long sides length 2*n.
 * https://oeis.org/A342914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13*n^2 + 9*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t term = nn2 * (seq_t)13 + nn * (seq_t)9 + (seq_t)2;
    return term / (seq_t)2;
}
