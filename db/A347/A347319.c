/*
 * A347319  a(n) = (2*n+1)*(n^3-2*n^2+n+1).
 * https://oeis.org/A347319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(n^3-2*n^2+n+1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term3 = sn * sn * sn - (seq_t)2 * sn * sn + sn + (seq_t)1;
    seq_t factor = (seq_t)2 * sn + (seq_t)1;
    return factor * term3;
}
