/*
 * A392928  Prime generating polynomial: a(n) = 32502 n^2 - 1727988 n +
 *          24009847.
 * https://oeis.org/A392928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 32502 n^2 - 1727988 n + 24009847
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;
    return 32502 * sq - 1727988 * sn + 24009847;
}
