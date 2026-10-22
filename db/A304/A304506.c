/*
 * A304506  a(n) = 2*(3*n+1)*(9*n+8).
 * https://oeis.org/A304506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(3*n+1)*(9*n+8)
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;        /* cast index to seq_t for all operations  */
    return (seq_t)2 * (sn * (seq_t)3 + (seq_t)1) *
                   (sn * (seq_t)9 + (seq_t)8);
}
