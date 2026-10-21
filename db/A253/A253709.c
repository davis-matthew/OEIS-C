/*
 * A253709  Integer squares c^2 that are equal to the sums of M (A253707)
 *          consecutive cubed integers equaling a squared integer, b^3 +
 *          (b+1)^3 + ... + (b+M-1)^3 = c^2, for a first term b(n) being an
 *          odd squared integer (A016754).
 * https://oeis.org/A253709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n(n+1)/2)(4(2n+1)^4-1))^2
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 22, u128 n <= 911,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                      /* convert index */
    seq_t t1 = (sn * (sn + 1)) / 2;           /* n(n+1)/2 */
    seq_t k  = (sn << 1) + 1;                 /* 2n+1 */
    seq_t k2 = k * k;                         /* (2n+1)^2 */
    seq_t k4 = k2 * k2;                       /* (2n+1)^4 */
    seq_t t2 = (4 * k4) - 1;                  /* 4*(2n+1)^4 - 1 */
    seq_t prod = t1 * t2;                     /* (n(n+1)/2)*(4(2n+1)^4-1) */
    return prod * prod;                      /* square the product */
}
