/*
 * A371094  a(n) = m*(2^e) + ((4^e)-1)/3, where m = 3n+1, and e is the 2-adic
 *          valuation of m.
 * https://oeis.org/A371094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m * (2^e) + ((4^e) - 1) / 3 where m = 3n + 1 and e =
 *            v2(m), the 2-adic valuation of m
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 9570, u64 n <= 9570,
 *            u128 n <= 9570, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = 3 * (seq_t)n + 1;            /* m = 3n + 1                       */
    idx_t  e = 0;                          /* exponent of 2 in m               */

    /* Count trailing zeros (2-adic valuation) */
    seq_t t = m;
    while ((t & 1) == 0) {                 /* while t even                     */
        e++;
        t >>= 1;                           /* shift right by one bit           */
    }

    seq_t pow2e = ((seq_t)1) << e;         /* 2^e                               */
    seq_t pow4e = pow2e * pow2e;           /* 4^e = (2^e)^2                     */

    /* Compute the formula */
    seq_t result = m * pow2e + (pow4e - 1) / 3;
    return result;
}
