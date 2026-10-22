/*
 * A329523  a(n) = n * (binomial(n + 1, 3) + 1).
 * https://oeis.org/A329523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(binomial(n+1,3)+1)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the compact polynomial form:
       a(n) = n * (n + 2) * (n^2 - 2*n + 3) / 6   (valid for n >= 0)   */
    seq_t nn   = (seq_t)n;
    seq_t nsq  = nn * nn;                            /* n^2            */
    seq_t term = nsq - 2 * nn + 3;                    /* n^2 - 2n + 3  */
    seq_t prod = nn * (nn + 2) * term;                /* n*(n+2)*(...) */
    return prod / 6;                                 /* divide by 6     */
}
