/*
 * A382809  a(n) = (6*n + 1)*(12*n + 1)*(18*n + 1).
 * https://oeis.org/A382809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 1)*(12*n + 1)*(18*n + 1)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term1 = 6 * sn + 1;
    seq_t term2 = 12 * sn + 1;
    seq_t term3 = 18 * sn + 1;
    return term1 * term2 * term3;
}
