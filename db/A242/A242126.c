/*
 * A242126  a(n) = n-th prime modulo 28.
 * https://oeis.org/A242126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime modulo 28
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for generated primes and the next odd candidate. */
    static uint64_t *primes      = NULL;
    static idx_t    primes_cnt   = 0;
    static idx_t    primes_cap   = 0;
    static uint64_t last_candidate = 3;  /* next odd number to test */

    if (n == 0) return 0;          /* undefined for indices below offset */

    /* Initialise the cache on the first call. */
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) return 0;   /* out-of-memory guard */
        primes[0] = 2;           /* the first prime */
        primes_cnt = 1;
    }

    /* Generate primes until we have at least n of them. */
    while (primes_cnt < n) {
        uint64_t cand   = last_candidate;
        bool     prime  = true;

        /* Test divisibility by all known primes up to sqrt(cand). */
        for (idx_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            /* Expand the cache if necessary. */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) return 0; /* out-of-memory guard */
                primes = tmp;
            }
            primes[primes_cnt++] = cand;
        }

        /* The next candidate is the next odd number. */
        last_candidate += 2;
    }

    /* Return the nth prime modulo 28 as a seq_t. */
    seq_t result = (seq_t)(primes[n - 1] % 28ULL);
    return result;
}
