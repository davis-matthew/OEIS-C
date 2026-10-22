/*
 * A304835  a(n) = 108*n^2 - 104*n + 20 (n>=1).
 * https://oeis.org/A304835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 108*n^2 - 104*n + 20 (for n >= 1)
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t before any arithmetic to avoid truncation */
    seq_t sn = (seq_t)n;
    seq_t term = (seq_t)108 * sn * sn - (seq_t)104 * sn + (seq_t)20;
    return term;
}
