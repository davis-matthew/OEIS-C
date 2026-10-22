/*
 * A322677  a(n) = 16*n*(n+1)*(2*n+1)^2.
 * https://oeis.org/A322677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n*(n+1)*(2*n+1)^2
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 90, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* cast index to seq_t */
    seq_t t2 = 2*nn + 1;                 /* (2*n + 1) */
    seq_t res = (seq_t)16 * nn * (nn + 1) * (t2 * t2);
    return res;
}
