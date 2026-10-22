/*
 * A301275  Numerator of variance of first n primes.
 * https://oeis.org/A301275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n * Σp_i^2 – (Σp_i)^2) / (n*(n‑1)) with the fraction
 *            reduced by gcd where p_i is the i‑th prime. The returned value
 *            is the numerator of the sample variance of the first n primes
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 190, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1)=0.  For n<2 the variance is undefined.  */
    if (n < 2) return 0;

    seq_t sum  = 0;        /* Σ p_i   */
    seq_t sum2 = 0;        /* Σ p_i^2 */

    for (idx_t i = 1; i <= n; ++i)
    {
        uint32_t p   = get_prime(i);   /* 1‑indexed primes */
        seq_t  sp    = (seq_t)p;
        sum      += sp;
        sum2     += sp * sp;
    }

    seq_t num = (seq_t)n * sum2 - sum * sum;       /* numerator of the fraction */
    seq_t den = (seq_t)n * (seq_t)(n - 1);          /* denominator of the fraction */
    seq_t g   = gcd(num, den);                     /* reduce the fraction */
    return num / g;
}
