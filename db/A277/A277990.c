/*
 * A277990  a(n) = 54*n^2 + 6*n.
 * https://oeis.org/A277990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*n^2 + 6*n
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return 54 * sn * sn + 6 * sn;
}
