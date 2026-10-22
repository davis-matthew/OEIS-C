/*
 * A304484  a(n) = A033270(n)*A033270(2n), where A033270 counts the odd
 *          primes.
 * https://oeis.org/A304484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A033270(n) * A033270(2n)
 * Range:     u8 n <= 41, u16 n <= 1166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033270(n) is the number of odd primes ≤ n  */
/* a(n) = (π(n)−[n≥2]) · (π(2n)−1)               */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static storage for prime counts up to the current limit */
    static uint64_t *prime_count = NULL;
    static uint64_t sieve_limit = 0;
    /* limit needed for this call (we need primes up to 2n) */
    uint64_t limit = (uint64_t)n * 2;

    /* recompute sieve if required */
    if (limit > sieve_limit) {
        /* allocate and compute prime flags up to limit */
        uint8_t *sieve = (uint8_t *)calloc(limit + 1, 1);
        if (!sieve) return 0;          /* out of memory → return 0 */
        sieve[0] = sieve[1] = 1;        /* not prime */

        uint64_t i, j;
        for (i = 2; i * i <= limit; ++i) {
            if (!sieve[i]) {
                for (j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }

        /* build prefix prime count array */
        uint64_t *new_count = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
        if (!new_count) { free(sieve); return 0; }

        uint64_t cnt = 0;
        for (i = 0; i <= limit; ++i) {
            if (i >= 2 && !sieve[i]) ++cnt;
            new_count[i] = cnt;
        }

        free(sieve);
        if (prime_count) free(prime_count);
        prime_count = new_count;
        sieve_limit = limit;
    }

    /* compute odd prime counts */
    uint64_t odd1 = prime_count[n] - (n >= 2 ? 1 : 0);
    /* 2n is always ≥ 2 for n ≥ 1, so subtract 1 */
    uint64_t odd2 = prime_count[limit] - 1;

    /* return product in seq_t */
    return (seq_t)odd1 * (seq_t)odd2;
}
