/*
 * A328181  a(n) = (-1)^(bigomega(n) - omega(n)) * Sum_{d|n} (-1)^(bigomega(d)
 *          - omega(d)) * d.
 * https://oeis.org/A328181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (-1)^(bigomega(n)-omega(n)) * Σ_{d|n}
 *            (-1)^(bigomega(d)-omega(d)) * d The function implements the
 *            multiplicative definition: a(p^e) = (p^(e+1) -
 *            (-1)^e*(2p+1))/(p+1) for e ≥ 1 a(1) = 1 a(n) = ∏_{p^e‖n} a(p^e)
 *            (product over prime powers dividing n) This implementation
 *            factors n by trial div...
 * Range:     u8 n <= 16, u16 n <= 1771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation for seq_t; exponent is small (≤ log_2(n)+1) */
static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* value of a(p^e) for e ≥ 1, p prime */
static seq_t a_prime_power(seq_t p, idx_t e)
{
    seq_t pow_val = power_seq_t(p, e + 1);        /* p^(e+1) */
    seq_t term  = (seq_t)(2 * p + 1);             /* (2p+1) */
    seq_t numerator;
    if ((e & 1) == 0)                            /* e even : (-1)^e = +1 */
        numerator = pow_val - term;              /* p^(e+1) - (2p+1) */
    else
        numerator = pow_val + term;              /* p^(e+1) + (2p+1) */
    return numerator / (p + 1);                  /* exact division */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for 0, defensive */

    idx_t k = n;                 /* working copy for factorisation */
    seq_t result = 1;            /* multiplicative identity */

    for (idx_t p = 2; p * p <= k; ++p)
    {
        if (k % p == 0)
        {
            idx_t e = 0;
            while (k % p == 0)
            {
                k /= p;
                ++e;
            }
            result *= a_prime_power((seq_t)p, e);
        }
    }

    /* remainder > 1 is a prime factor with exponent 1 */
    if (k > 1)
        result *= a_prime_power((seq_t)k, 1);

    return result;
}
