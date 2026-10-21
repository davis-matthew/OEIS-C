/*
 * A245784  Numerator of (n/tau(n) + sigma(n)/n).
 * https://oeis.org/A245784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (n / tau(n) + sigma(n) / n) where * tau(n)
 *            is the divisor count and sigma(n) is the sum of divisors
 * Range:     u8 n <= 11, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

/* Compute gcd for seq_t type. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute power: base^exp (exp is small <= 64). */
static seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (unused, but safe). */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t tau = (seq_t)1;   /* divisor count */
    seq_t sigma = (seq_t)1; /* sum of divisors */

    uint64_t temp = (uint64_t)n;

    /* Factorization by trial division. */
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            unsigned int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            /* Update tau. */
            tau *= (seq_t)(exp + 1);

            /* Update sigma: (p^(exp+1)-1)/(p-1). */
            seq_t p_seq = (seq_t)p;
            seq_t pow_val = pow_seq(p_seq, exp + 1);
            seq_t sigma_factor = (pow_val - (seq_t)1) / (p_seq - (seq_t)1);
            sigma *= sigma_factor;
        }
    }

    /* Remaining prime factor if any. */
    if (temp > 1)
    {
        uint64_t p = temp;
        /* exponent = 1 */
        tau *= (seq_t)2;
        seq_t p_seq = (seq_t)p;
        seq_t pow_val = pow_seq(p_seq, 2);
        seq_t sigma_factor = (pow_val - (seq_t)1) / (p_seq - (seq_t)1);
        sigma *= sigma_factor;
    }

    /* Compute numerator and denominator of the fraction. */
    seq_t num = nn * nn + sigma * tau;
    seq_t den = tau * nn;

    /* Reduce fraction to lowest terms. */
    seq_t g = gcd_seq_t(num, den);
    return num / g;
}
