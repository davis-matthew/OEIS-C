/*
 * A348061  a(n) = Sum_{k=1..n, gcd(n,k) = 1} n / gcd(n,k-1).
 * https://oeis.org/A348061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} n / gcd(n,k-1) (Offset 1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 3108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   term_of_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/term_of_prime.h"

/*  Computes a(n) for all n >= 1   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 for safety */

    seq_t result = 1;
    idx_t m = n;

    /* trial division for each potential prime divisor */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            unsigned e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result *= term_of_prime(p, e);
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1)
        result *= term_of_prime(m, 1);

    return result;
}
