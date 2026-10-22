/*
 * A365649  Dirichlet convolution of sigma with Dedekind psi function.
 * https://oeis.org/A365649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Σ_{d|n} σ(d) * ψ(n/d) (σ = sum‑of‑divisors, ψ = Dedekind
 *            psi) Multiplicative: a(p^e) = (2 + ((e+1)*p^2 – 2p – e – 1)*p^e)
 *            / (p – 1)^2 Uses the closed form above to evaluate a(n) as a
 *            product over prime powers
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 2028, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a_prime_power(idx_t p, idx_t e)
{
    /* compute p^e */
    seq_t pow_pe = 1;
    for (idx_t i = 0; i < e; ++i)
        pow_pe *= (seq_t)p;

    /* compute term = ((e+1)*p^2 – 2p – e – 1) */
    seq_t psq = (seq_t)p * (seq_t)p;
    seq_t term = ((seq_t)(e + 1)) * psq;
    term -= (seq_t)(2 * p);
    term -= (seq_t) e;
    term -= (seq_t) 1;

    /* numerator = 2 + term * p^e */
    seq_t numerator = 2 + term * pow_pe;

    /* denominator = (p – 1)^2 */
    seq_t denom = (seq_t)(p - 1);
    denom *= denom;

    return numerator / denom;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* values for n < 1 are undefined */
    if (n == 1) return 1;          /* empty product convention */

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p <= m / p; ++p) /* trial division */
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result *= a_prime_power(p, e);
        }
    }

    if (m > 1)          /* remaining prime factor with exponent 1 */
        result *= a_prime_power(m, 1);

    return result;
}
