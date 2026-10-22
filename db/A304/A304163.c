/*
 * A304163  a(n) = 9*n^2 - 3*n + 1 with n>0.
 * https://oeis.org/A304163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 3*n + 1 (n > 0)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (seq_t)9 * tn * tn - (seq_t)3 * tn + 1;
}
