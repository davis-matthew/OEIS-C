/*
 * A264892  a(n) = n*(3*n - 2)*(9*n^2 - 6*n - 2).
 * https://oeis.org/A264892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n - 2)*(9*n^2 - 6*n - 2)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 112, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t part1 = (seq_t)3 * tn - (seq_t)2;
    seq_t part2 = (seq_t)9 * tn * tn - (seq_t)6 * tn - (seq_t)2;
    return tn * part1 * part2;
}
