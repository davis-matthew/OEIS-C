/*
 * A373060  a(n) = Sum_{1 <= x_1, x_2, x_3 <= n} gcd(x_1, n)/gcd(x_1, x_2,
 *          x_3, n).
 * https://oeis.org/A373060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{1 ≤ x₁,x₂,x₃ ≤ n} gcd(x₁,n) / gcd(x₁,x₂,x₃,n) (OEIS
 *            A373060)
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 779, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small power routine for seq_t. */
static seq_t pow_seq_small(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Multiplicative contribution of a prime power p^e. */
static seq_t ape_prime_power(uint64_t p, unsigned e)
{
    seq_t total = 0;
    seq_t sp   = (seq_t)p;
    for (unsigned i = 0; i <= e; ++i)
    {
        seq_t fi = (i == e) ? sp : (sp - 1);
        for (unsigned j = 0; j <= e; ++j)
        {
            seq_t fj = (j == e) ? sp : (sp - 1);
            for (unsigned k = 0; k <= e; ++k)
            {
                seq_t fk = (k == e) ? sp : (sp - 1);

                /* exponent in the formula: 3e - 3 - min(i,j,k) - j - k */
                unsigned min_ijk = i < j ? (i < k ? i : k) : (j < k ? j : k);
                int expo = 3 * (int)e - 3 - (int)min_ijk - (int)j - (int)k;

                seq_t term = fi * fj * fk;
                if (expo > 0)
                    term *= pow_seq_small(sp, (unsigned)expo);
                else if (expo < 0)
                    term /= pow_seq_small(sp, (unsigned)(-expo));
                /* expo == 0 leaves term unchanged */

                total += term;
            }
        }
    }
    return total;
}

/* Helper structure for factorisation. */
struct factor
{
    uint64_t prime;
    unsigned exp;
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    uint64_t m = (uint64_t)n;
    struct factor fac[64];
    int fac_cnt = 0;

    /* handle factor 2 */
    if ((m & 1U) == 0)
    {
        unsigned e = 0;
        while ((m & 1U) == 0)
        {
            m >>= 1;
            ++e;
        }
        fac[fac_cnt++] = (struct factor){ 2ULL, e };
    }

    /* odd primes */
    for (uint64_t d = 3; d * d <= m; d += 2)
    {
        if (m % d == 0)
        {
            unsigned e = 0;
            while (m % d == 0)
            {
                m /= d;
                ++e;
            }
            fac[fac_cnt++] = (struct factor){ d, e };
        }
    }

    /* remaining prime factor */
    if (m > 1)
        fac[fac_cnt++] = (struct factor){ m, 1 };

    /* combine multiplicative values */
    seq_t result = 1;
    for (int i = 0; i < fac_cnt; ++i)
        result *= ape_prime_power(fac[i].prime, fac[i].exp);

    return result;
}
