/*
 * A387439  a(n) = (2*n + 1)*(12 + 11*n + 93*n^2 + 4*n^3)/3.
 * https://oeis.org/A387439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*(12 + 11*n + 93*n^2 + 4*n^3)/3
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sn_sq = sn * sn;
    seq_t sn_cube = sn_sq * sn;
    seq_t term1 = (seq_t)2 * sn + (seq_t)1;
    seq_t term2 = (seq_t)12
                + (seq_t)11 * sn
                + (seq_t)93 * sn_sq
                + (seq_t)4  * sn_cube;
    return term1 * term2 / (seq_t)3;
}
