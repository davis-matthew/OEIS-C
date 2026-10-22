/*
 * A399192  a(n) = number of integers k <= n with g(k)^4 < k, where g(k) is
 *          the largest divisor d of k satisfying d <= sqrt(k) (A033676).
 * https://oeis.org/A399192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers k ≤ n with g(k)^4 < k, where g(k) is
 *            the largest divisor of k not exceeding √k
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   fourth_root_of_cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses a sieve to pre‑compute the largest prime factor of each
    integer up to the maximum n requested.  Then for each k we test
    lpf(k) > floor( (k^3)^(1/4) ), which is equivalent to g(k)^4 < k. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/fourth_root_of_cube.h"

/* Static data for memoisation */
static uint64_t *largest_prime_factor = NULL;   /* array indexed by k */
static idx_t  precomputed_limit = 0;            /* largest n already processed */

/* Rebuild sieve up to 'limit' and compute largest prime factors. */
static void rebuild_sieve(idx_t limit)
{
    /* Allocate array for largest prime factor */
    uint64_t *lpf = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!lpf) exit(1);   /* abort on allocation failure */

    for (idx_t i = 0; i <= limit; ++i) lpf[i] = 0;
    lpf[1] = 1;   /* convention */

    /* Simple Eratosthenes sieve: generate primes and set lpf */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        /* Mark multiples of p and set lpf to current prime p. */
        for (uint64_t m = p; m <= limit; m += p) {
            is_prime[(size_t)m] = 0;
            lpf[(size_t)m] = (uint64_t)p;   /* larger primes overwrite earlier ones */
        }
    }

    free(is_prime);

    /* Free any previous data and store new data */
    if (largest_prime_factor) free(largest_prime_factor);
    largest_prime_factor = lpf;
    precomputed_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure preprocessing up to n */
    if (n > precomputed_limit) rebuild_sieve(n);

    seq_t count = 0;
    for (idx_t k = 2; k <= n; ++k) {
        uint64_t lp = largest_prime_factor[(size_t)k];
        if (lp > fourth_root_of_cube(k))
            count++;
    }
    return count;
}
