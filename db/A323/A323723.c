/*
 * A323723  a(n) = (-2 - (-1)^n*(-2 + n) + n + 2*n^3)/4.
 * https://oeis.org/A323723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-2 - (-1)^n*(-2 + n) + n + 2*n^3)/4
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: a(n) = n^3/2          if n even        */
/*                 a(n) = (n-1)*(n^2+n+2)/2 if n odd   */

seq_t a(idx_t n)
{
    const seq_t sn = (seq_t)n;               /* cast index to seq_t */

    if ((n & 1ULL) == 0)                     /* even n: a(n) = n^3/2 */
    {
        seq_t t = sn * sn * sn;              /* n^3   (fits seq_t for all n where a(n) fits) */
        t >>= 1;                             /* divide by 2 */
        return t;
    }
    else                                     /* odd n: a(n) = (n-1)*(n^2+n+2)/2 */
    {
        seq_t nsq = sn * sn;                 /* n^2  */
        seq_t t   = (sn - 1) * (nsq + sn + 2);
        t >>= 1;                             /* divide by 2 */
        return t;
    }
}
