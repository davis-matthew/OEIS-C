/*
 * A363289  Sum of the divisor complements of the unitary squarefree divisors
 *          of n.
 * https://oeis.org/A363289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * Σ_{d|n, gcd(d,n/d)=1} μ(d)^2 / d This simplifies to
 *            a(n) = n * Π_{p|n, ord_p(n)=1} (1 + 1/p)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* only defined for n≥1, but guard anyway */

    seq_t result = (seq_t)n;          /* start with n itself */
    idx_t temp = n;                  /* copy to factorize */

    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {   /* count exponent of p in n */
                ++exp;
                temp /= p;
            }
            if (exp == 1) {           /* only primes with exponent 1 contribute */
                result = result / (seq_t)p;          /* divide by p once */
                result = result * ((seq_t)p + 1);    /* multiply by (p+1) */
            }
        }
    }

    /* if a prime factor > sqrt(n) remains, its exponent is 1 */
    if (temp > 1) {
        result = result / (seq_t)temp;
        result = result * ((seq_t)temp + 1);
    }

    return result;
}
