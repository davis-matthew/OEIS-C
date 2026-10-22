/*
 * A397412  Dirichlet g.f.: zeta(s) * Product_{p prime} (1 - 1/p^s +
 *          1/p^(2*s-1)).
 * https://oeis.org/A397412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   multiplicative with a(p)=0, a(p^e)=p for e>=2. So a(n) = 1 if
 *            n==1; if n has any prime factor to exponent 1 → 0; otherwise
 *            a(n) = product of all distinct prime factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t temp = n;
    seq_t prod  = 1;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp == 1)          /* factor p appears only once → a(n)=0 */
                return (seq_t)0;
            prod *= (seq_t)p;       /* p contributes once to the product */
        }
    }

    /* if a prime > sqrt(original n) remains */
    if (temp > 1)                 /* remaining prime factor exponent is 1 */
        return (seq_t)0;

    return prod;
}
