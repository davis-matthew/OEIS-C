/*
 * A331429  Expansion of x^2*(10-5*x+x^2)/((1-x)^4*(1-x^2)).
 * https://oeis.org/A331429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+3)*(n^2+3*n-2)+4*(-1)^n-4)/8
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 324, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                      /* cast index to seq_t */
    seq_t prod = sn * (sn + 3) * (sn * sn + 3 * sn - 2);
    if (n & 1)                                 /* n is odd -> subtract 8 */
        prod -= (seq_t)8;
    return prod / 8;
}
