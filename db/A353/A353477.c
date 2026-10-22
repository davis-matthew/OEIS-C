/*
 * A353477  a(n) = 1 if n is a semiprime of the form 4k+1, otherwise 0.
 * https://oeis.org/A353477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a product of exactly two primes (counting
 *            multiplicity) and n ≡ 1 (mod 4); otherwise a(n) = 0
 * Range:     u8 n <= 9881, u16 n <= 9881, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Basic sanity checks: n must be at least 4 (the smallest semiprime 2×2 = 4) */
    if (n < 4)
        return 0;

    /* n must be congruent to 1 modulo 4 */
    if ((n & 3) != 1)
        return 0;

    /* Find the smallest prime factor p of n */
    idx_t p;
    for (p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)               /* p divides n */
        {
            idx_t q = n / p;           /* co‑factor */
            /* If q is prime we have exactly two prime factors (p and q)
               and n ≡ 1 (mod 4). Otherwise n has more than two factors. */
            return is_prime(q) ? 1 : 0;
        }
    }

    /* If no divisor found, n is prime (or 1) → not a semiprime */
    return 0;
}
