/*
 * A254474  30-gonal numbers: a(n) = n*(14*n-13).
 * https://oeis.org/A254474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(14*n-13)
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * ((seq_t)14 * sn - (seq_t)13);
}
