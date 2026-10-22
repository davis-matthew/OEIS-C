/*
 * A373499  a(n) = Sum_{i=1..n-1} binomial(prime(n),prime(i)).
 * https://oeis.org/A373499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n-1} binomial(prime(n),prime(i))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/sieve_first_n_primes.h"

static uint32_t *cached_primes = NULL;
static size_t cached_prime_count = 0;
static idx_t cached_max_n = 0;

/* Compute a(n) for the OEIS sequence A373499 */
seq_t a(idx_t n)
{
    /* Base case: empty sum */
    if (n <= 1)
        return 0;

    /* If we have not cached enough primes, rebuild the cache */
    if (n > cached_max_n) {
        /* Free old cache if exists */
        if (cached_primes != NULL)
            free(cached_primes);

        /* Sieve the first 'n' primes */
        uint32_t *p = NULL;
        size_t cnt = 0;
        sieve_first_n_primes(n, &p, &cnt);

        /* Store in static cache */
        cached_primes = p;
        cached_prime_count = cnt;
        cached_max_n = n;
    }

    /* prime(n) is the n-th prime (1‑based) */
    idx_t prime_n = (idx_t)cached_primes[n - 1];

    seq_t sum = 0;
    for (idx_t i = 1; i < n; ++i) {
        idx_t prime_i = (idx_t)cached_primes[i - 1];
        sum += binomial(prime_n, prime_i);
    }

    return sum;
}
