/*
 * A349416  a(n) is the Wiener index of a broom on 2n vertices of which n+2
 *          are pendant.
 * https://oeis.org/A349416
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 3*n^2 + 5*n)/6
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t numerator = (seq_t)4 * n3 + (seq_t)3 * n2 + (seq_t)5 * sn;
    return numerator / 6;
}
