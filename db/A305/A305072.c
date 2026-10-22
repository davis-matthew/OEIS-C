/*
 * A305072  a(n) = 144*n^2 - 24*n (n>=1).
 * https://oeis.org/A305072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*n^2 - 24*n (n>=1)
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset is 1; undefined for n<1 */
    seq_t nn = (seq_t)n;               /* cast index to seq_t first */
    return (seq_t)144 * nn * nn - (seq_t)24 * nn;
}
