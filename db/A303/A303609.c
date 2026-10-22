/*
 * A303609  a(n) = 2*n^3 + 9*n^2 + 9*n.
 * https://oeis.org/A303609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 + 9*n^2 + 9*n
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return N * (N * ((seq_t)2 * N + (seq_t)9) + (seq_t)9);
}
