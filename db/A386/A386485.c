/*
 * A386485  a(0) = 1; thereafter a(n) = 5*n^2 - 5*n + 2.
 * https://oeis.org/A386485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; thereafter a(n) = 5*n^2 - 5*n + 2 for n ≥ 1
 * Range:     u8 n <= 7, u16 n <= 114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    seq_t nn = (seq_t)n;          /* convert index to the sequence type */
    seq_t ans = 5 * nn * nn;      /* 5 * n^2                     */
    ans -= 5 * nn;                /* subtract 5 * n               */
    ans += 2;                     /* add constant                 */
    return ans;
}
