/*
 * A304505  a(n) = 4*(n+1)*(9*n+4).
 * https://oeis.org/A304505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n+1)*(9*n+4)
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t term = 4 * (nn + 1) * (9 * nn + 4);
    return term;
}
