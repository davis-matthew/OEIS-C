/*
 * A336563  Sum of proper divisors of n that are divisible by every prime that
 *          divides n.
 * https://oeis.org/A336563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n) * (sigma(n/rad(n)) - n/rad(n)) where rad(n) =
 *            product of distinct primes of n. This uses prime factorisation
 *            of n
 * Range:     u8 n <= 215, u16 n <= 9215, u32 n <= 9215, u64 n <= 9215,
 *            u128 n <= 9215, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;   /* array of primes */
static size_t prime_cnt = 0;      /* number of primes in array    */
static idx_t  prime_limit = 0;    /* largest n for which primes are built */

/* ensure that primes[0..prime_cnt-1] contains all primes <= n */
static void ensure_primes(idx_t n)
{
    if (n <= prime_limit) return;
    if (primes) free(primes);
    build_primes_upto(n, &primes, &prime_cnt);
    prime_limit = n;
}

seq_t a(idx_t n)
{
    /* Treat n==0 as 0 (outside the offset) */
    if (n == 0) return (seq_t)0;
    /* For n>=1 */
    uint64_t temp = (uint64_t)n;
    /* ensure we have enough primes for factorisation */
    ensure_primes((idx_t)n);

    seq_t rad   = (seq_t)1;      /* product of distinct primes */
    seq_t sigma = (seq_t)1;      /* product of sigma(p^(e-1)) of m */

    /* factor n with trial division */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;  /* p > sqrt(temp) */
        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp == 0) continue;

        rad *= (seq_t)p;   /* multiply rad by p once */

        if (exp > 1) {
            /* compute p^exp (needed for sigma(p^(exp-1))) */
            seq_t p_pow = (seq_t)1;
            for (unsigned k = 0; k < exp; ++k)
                p_pow *= (seq_t)p;           /* p_pow = p^exp */

            /* sigma(p^(exp-1)) = (p^exp - 1) / (p - 1) */
            seq_t term = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma *= term;
        }
        /* if exp == 1, p contributes nothing to sigma (factor = 1) */
    }

    /* if a prime factor remains > sqrt(n) */
    if (temp > 1) {
        rad *= (seq_t)temp;        /* include it in rad */
        /* remaining exponent is 1, so no contribution to sigma */
    }

    seq_t m = (seq_t)n / rad;      /* m = n / rad(n) */
    seq_t result = rad * (sigma - m);   /* a(n) = rad * (sigma(m) - m) */

    return result;
}
