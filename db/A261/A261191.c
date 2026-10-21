/*
 * A261191  40-gonal numbers: a(n) = 38*n*(n-1)/2 + n.
 * https://oeis.org/A261191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(19*n - 18)
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * ((seq_t)19 * sn - (seq_t)18);
}
