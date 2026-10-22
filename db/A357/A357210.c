/*
 * A357210  a(n) = Sum_{k=1..n} prime(k/gcd(n,k)).
 * https://oeis.org/A357210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(k/gcd(n,k)) (where prime(i) is the
 *            i-th prime, 1‑based) The implementation uses a simple sieve that
 *            is rebuilt when a larger number of primes is required
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   _idx_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_idx_gcd.h"

seq_t a(idx_t n)
{
    /* Static state for the prime sieve */
    static bool initialized = false;
    static idx_t prime_limit = 0;            /* largest number sieved */
    static uint64_t *prime_list = NULL;      /* array of primes (1‑based index) */
    static size_t prime_count = 0;           /* number of primes found */

    /* If this is the first call or n requires a larger sieve, rebuild it. */
    if (!initialized || ((size_t)prime_count < (size_t)n)) {
        /* Choose a sieve limit that surely contains at least n primes
           (n*20+100 guarantees for n≤10000). */
        idx_t limit = n * 20 + 100;
        if (limit <= prime_limit) {
            limit = prime_limit + 1;         /* avoid shrinking */
        }

        /* allocate boolean array */
        unsigned char *is_prime = (unsigned char*)calloc((size_t)(limit + 1), 1);
        if (!is_prime) return 0;            /* failure fallback */

        /* Sieve of Eratosthenes */
        is_prime[0] = 0;
        if (limit >= 1) is_prime[1] = 0;
        for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;
        for (idx_t i = 2; (i * i) <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count primes */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) ++cnt;
        }

        /* Allocate prime list */
        uint64_t *primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
        if (!primes) {
            free(is_prime);
            return 0;
        }

        /* Store primes */
        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                primes[idx++] = (uint64_t)i;
            }
        }

        /* Release old data and update static state */
        free(is_prime);
        free(prime_list);
        prime_list  = primes;
        prime_count = cnt;
        prime_limit = limit;
        initialized = true;
    }

    /* Sum over k */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t g = _idx_gcd(n, k);          /* gcd(n,k) */
        idx_t h = k / g;                    /* argument for prime() */
        /* h is in [1..n]; prime_list is 0‑based, so access [h-1] */
        sum += (seq_t)prime_list[(size_t)(h - 1)];
    }
    return sum;
}
