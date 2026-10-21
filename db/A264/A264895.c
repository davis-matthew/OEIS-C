/*
 * A264895  a(n) = n*(4*n - 3)*(16*n^2 - 12*n - 3).
 * https://oeis.org/A264895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n - 3)*(16*n^2 - 12*n - 3)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 90, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t part1 = tn * ((seq_t)(4 * tn - 3));
    seq_t part2 = ((seq_t)16 * tn * tn - (seq_t)12 * tn - (seq_t)3);
    return part1 * part2;
}
