/*
 * A391187  a(n) is the smallest positive integer such that a(n) * n is a
 *          number of the form x^2 + 2*y^2 (A002479).
 * https://oeis.org/A391187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p|n, p ≡5 or 7 (mod 8)} p^{e mod 2} (e = exponent of p
 *            in n)
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest positive integer m such that m*n is representable as x^2+2*y^2.
   By the multiplicative formula, a(p^e)=p^{e mod 2} if p≡5 or 7 (mod 8), otherwise 1. */
seq_t a(idx_t n)
{
    if (n < 1)    /* undefined for n=0, but return 0 as a safe fallback */
        return 0;

    seq_t result = 1;
    idx_t m = n;          /* remaining part to factorize */

    for (idx_t p = 2; p <= m / p; ++p)   /* use division to avoid overflow */
    {
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            if ((p % 8 == 5) || (p % 8 == 7))
                if (exp & 1)                /* e mod 2 == 1 */
                    result *= (seq_t)p;
        }
    }
    if (m > 1) /* m is a prime factor > sqrt(original n) */
    {
        if ((m % 8 == 5) || (m % 8 == 7))
            result *= (seq_t)m; /* exponent is 1, so e mod 2 == 1 */
    }
    return result;
}
