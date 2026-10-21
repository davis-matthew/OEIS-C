/*
 * A272357  a(n) = n*(105*n^3 - 210*n^2 + 147*n - 34).
 * https://oeis.org/A272357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(105*n^3 - 210*n^2 + 147*n - 34)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 80, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t term = (seq_t)105 * n4
               - (seq_t)210 * n3
               + (seq_t)147 * n2
               - (seq_t)34  * N;
    return term;
}
