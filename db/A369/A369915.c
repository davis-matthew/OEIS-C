/*
 * A369915  a(n) = Sum_{p|n, p prime} lcm(p, n/p) / p.
 * https://oeis.org/A369915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} lcm(p, n/p)/p
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 */

    seq_t nseq   = (seq_t)n;              /* n as seq_t */
    seq_t result = (seq_t)0;

    uint64_t m = n;                       /* mutable copy for factorisation */

    /* trial division – suitable for the limits of the tests */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)                   /* p is a prime divisor of n */
        {
            int exp = 0;
            while (m % p == 0)             /* count the exponent of p */
            {
                m /= p;
                ++exp;
            }

            seq_t denom;
            if (exp == 1)
                denom = (seq_t)p;          /* term = n/p           */
            else
                denom = (seq_t)p * (seq_t)p; /* term = n/p²          */

            result += nseq / denom;
        }
    }

    /* the remaining m, if >1, is a prime factor with exponent 1 */
    if (m > 1)
        result += nseq / (seq_t)m;          /* term = n/m   */

    return result;
}
