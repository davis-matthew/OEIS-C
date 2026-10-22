/*
 * A351112  Number of balanced numbers dividing n.
 * https://oeis.org/A351112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, phi(d) | sigma(d)} 1 (Number of balanced
 *            divisors of n)
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ----------------------------------------------------------------------
 * Helper: Euler's totient function φ(n) for n ≥ 1.
 * Uses trial division; works for any seq_t that supports ordinary
 * arithmetic with small integers.
 * ---------------------------------------------------------------------- */
static seq_t phi_of(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t result = (seq_t)n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0) m /= p;
            result -= result / (seq_t)p;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* ----------------------------------------------------------------------
 * Helper: σ(n), the sum of divisors of n for n ≥ 1.
 * Uses multiplicativity: for each prime power p^e the factor is
 *          (p^(e+1) - 1) / (p - 1)
 * Implementation with prime factorisation via trial division.
 * ---------------------------------------------------------------------- */
static seq_t sigma_of(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t result = (seq_t)1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            seq_t term = (seq_t)1;      /* current (p^(e+1)-1)/(p-1) factor */
            seq_t pk    = (seq_t)1;      /* p^e */
            do
            {
                m /= p;
                pk *= (seq_t)p;
                term += pk;
            } while (m % p == 0);
            result *= term;
        }
    }
    if (m > 1) /* remaining prime factor */
    {
        result *= (seq_t)(1 + m);
    }
    return result;
}

/* ----------------------------------------------------------------------
 * Main sequence function
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    seq_t count = (seq_t)0;

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            seq_t phi_d  = phi_of(d);
            seq_t sigma_d = sigma_of(d);
            if (sigma_d % phi_d == 0) ++count;

            /* paired divisor n/d, avoid double counting when d == n/d */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t phi_d2  = phi_of(d2);
                seq_t sigma_d2 = sigma_of(d2);
                if (sigma_d2 % phi_d2 == 0) ++count;
            }
        }
    }

    return count;
}
