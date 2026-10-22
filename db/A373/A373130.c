/*
 * A373130  a(n) = Sum_{1 <= x_1, x_2, x_3 <= n} sigma( n/gcd(x_1, x_2, x_3,
 *          n) ).
 * https://oeis.org/A373130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from OEIS A373130.  
   Formula: a(n) = Σ_{d|n} J_3(d) * σ(d)   (Jordan totient J_3)  
   Since a(n) is multiplicative, for n = p^e  
   a(p^e) = (p^(4e+2)*(p^2+p+1) - p^(3e)*(p^3+p^2+p+1) + p) / (p^4-1). */

#include <stdint.h>
#include "../utils/power128.h"

/* Compute a(p^e) for a prime p and exponent e ≥ 1.  All arithmetic is
   performed in 128‑bit to avoid overflow of intermediate terms. */
static seq_t a_prime_power(idx_t p, idx_t e)
{
    unsigned __int128 pp = p;
    unsigned __int128 p2 = pp * pp;       /* p^2   */
    unsigned __int128 p3 = p2 * pp;       /* p^3   */
    unsigned __int128 p4 = p2 * p2;       /* p^4   */

    /* p^(4e+2) and p^(3e) using 128‑bit */
    unsigned __int128 pow4e2 = power128(pp, 4 * e + 2);
    unsigned __int128 pow3e  = power128(pp, 3 * e);

    /* numerator = p^(4e+2)*(p^2+p+1) - p^(3e)*(p^3+p^2+p+1) + p */
    unsigned __int128 term1 = pow4e2 * (p2 + pp + (unsigned __int128)1);
    unsigned __int128 term2 = pow3e  * (p3 + p2 + pp + (unsigned __int128)1);
    unsigned __int128 numerator = term1 - term2 + pp;

    /* denominator = p^4 - 1 */
    unsigned __int128 denominator = p4 - (unsigned __int128)1;

    unsigned __int128 result128 = numerator / denominator;
    return (seq_t) result128;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    idx_t m = n;
    seq_t result = (seq_t)1;

    for (idx_t p = 2; p * p <= m; ++p)
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

    /* If remaining factor is prime > sqrt(n) */
    if (m > 1)
    {
        result *= a_prime_power(m, 1);
    }

    return result;
}
