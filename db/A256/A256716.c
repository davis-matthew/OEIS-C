/*
 * A256716  a(n) = n*(n+1)*(22*n-19)/6.
 * https://oeis.org/A256716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(22*n-19)/6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 589, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;            /* cast index to seq_t */
    seq_t t1  = nn + 1;               /* (n + 1)   */
    seq_t t2  = (seq_t)22 * nn - (seq_t)19;  /* (22*n - 19) */
    seq_t prod = nn * t1 * t2;        /* n*(n+1)*(22*n-19) */
    return prod / (seq_t)6;           /* divide by 6 */
}
