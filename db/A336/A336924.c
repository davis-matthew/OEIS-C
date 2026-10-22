/*
 * A336924  a(n) = spf(1+sigma(n)), where spf is the smallest prime factor and
 *          sigma is the sum of divisors function.
 * https://oeis.org/A336924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = spf(1 + sigma(n))
 * Range:     u8 n <= 107, u16 n <= 9500, u32 n <= 9500, u64 n <= 9500,
 *            u128 n <= 9500, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where spf is the smallest prime factor and sigma is the sum of divisors function. */
#include <stdint.h>
#include "../utils/sigma_of.h"

/* Simple routine to compute the smallest prime factor of a number.
   Works for any seq_t that supports basic arithmetic operators. */
static seq_t smallest_prime_factor(seq_t x)
{
    seq_t n = x;
    if (n <= 1) return n;              /* for completeness */
    if ((n & 1) == 0) return 2;        /* even numbers */

    /* test odd candidates up to sqrt(n) */
    for (idx_t p = 3; (seq_t)p <= n / (seq_t)p; p += 2) {
        if (n % (seq_t)p == 0)
            return (seq_t)p;           /* found smallest factor */
    }
    return n;                          /* n is prime */
}

/* Compute a(n) = spf(1 + sigma(n)) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* behaviour below the offset is irrelevant */

    seq_t sigma = sigma_of(n);                /* sigma(n) */
    seq_t s = sigma + 1;                      /* 1 + sigma(n) */

    return smallest_prime_factor(s);
}
