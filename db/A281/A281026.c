/*
 * A281026  a(n) = floor(3*n*(n+1)/4).
 * https://oeis.org/A281026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n*(n+1)/4)
 * Range:     u8 n <= 8, u16 n <= 238, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);   /* n*(n+1) */
    prod = prod * (seq_t)3;       /* 3*n*(n+1) */
    prod = prod / (seq_t)4;       /* floor division by 4 */
    return prod;
}
