/*
 * A256666  a(n) = ( 2*n*(2*n^2 + 11*n + 26) - (-1)^n + 1 )/16.
 * https://oeis.org/A256666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( 2*n*(2*n^2 + 11*n + 26) - (-1)^n + 1 )/16
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for computation */
    seq_t sn = (seq_t)n;
    seq_t nn = sn * sn;                           /* n^2 */
    seq_t t  = 2 * nn + 11 * sn + 26;             /* 2*n^2 + 11*n + 26 */
    seq_t term = 2 * sn * t;                       /* 2*n*(...) */

    /* Numerator: term + 1 - (-1)^n */
    seq_t num = term + 1;
    if ((n & 1) == 0)           /* n even: (-1)^n = 1, subtract 1 */
        num -= 1;
    else                        /* n odd: (-1)^n = -1, subtract (-1) = +1 */
        num += 1;

    /* Divide by 16 */
    return num / 16;
}
