/*
 * A256646  26-gonal pyramidal numbers: a(n) = n*(n+1)*(8*n-7)/2.
 * https://oeis.org/A256646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(8*n-7)/2
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 903, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = sn * (sn + (seq_t)1) * ((seq_t)8 * sn - (seq_t)7) / (seq_t)2;
    return term;
}
