/*
 * A344687  a(n) is the lowest nonnegative exponent k such that n!^k is the
 *          product of the divisors of n!.
 * https://oeis.org/A344687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n!)/2
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here d(m) is the divisor‑count function.  For n = 1 we return 0. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Compute a(n) for sequence A344687.
 * The function is safe for any n >= 1 and works with the integer
 * types seq_t and idx_t supplied by the build environment. */
seq_t a(idx_t n)
{
    /* The trivial case: a(1) = 0 (since 1! has only one divisor). */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Allocate an array that will hold the primality flags up to n. */
    /* Use an array of uint8_t because standard C has no builtin bool type. */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)n + 1, sizeof(uint8_t));
    if (!is_prime) {
        /* In case of memory exhaustion we return 0, treating it as a fatal error. */
        return (seq_t)0;
    }

    /* Initially mark all numbers as prime except 0 and 1. */
    memset(is_prime + 2, 1, (size_t)n - 1);  /* set indices 2..n to 1 */

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i <= n / i; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Compute d(n!) = product_{p<=n} (e_p(n)+1), where e_p(n) is
       the exponent of prime p in n!. */
    seq_t d = (seq_t)1;
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p]) {
            continue; /* not a prime */
        }

        /* Compute the exponent of prime p in n! using Legendre's formula. */
        idx_t exp = 0;
        for (idx_t power = p; power <= n; power *= p) {
            exp += n / power;
            /* Prevent overflow of 'power' for large p. */
            if (power > n / p) {
                break;
            }
        }

        /* Update the divisor count product. */
        d = d * (seq_t)(exp + 1);
    }

    free(is_prime);

    /* a(n) = d(n!)/2.  For n>1 the product d(n!) is always even. */
    return d / 2;
}
