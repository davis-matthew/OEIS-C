/*
 * A277984  a(n) = 6*n*(9*n-5).
 * https://oeis.org/A277984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*(9*n-5)
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 6 * N * (9 * N - 5);
}
