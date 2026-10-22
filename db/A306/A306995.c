/*
 * A306995  Number of primes q < prime(n), such that 2*prime(n) + q is prime.
 * https://oeis.org/A306995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q < prime(n) with 2*prime(n)+q prime
 * Range:     u8 n <= 24, u16 n <= 3259, u32 n <= 7491, u64 n <= 7491,
 *            u128 n <= 7491, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                /* malloc, free */
#include "../utils/get_prime.h"    /* get_prime(idx_t) -> uint32_t */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t**, size_t*) */
#include "../utils/is_prime_small.h"     /* is_prime_small(uint64_t) -> bool */

/* Cache of primes up to maxLimit */
static idx_t        maxPrimeLimit = 0;   /* largest limit for which primes are cached */
static uint64_t    *primeCache    = NULL; /* array of primes (ascending) */
static size_t       primeCount     = 0;    /* number of primes in primeCache */

/* Ensure that primeCache contains all primes <= limit */
static void
ensure_primes_up_to(idx_t limit)
{
    if (limit <= maxPrimeLimit) return;        /* already have enough */

    /* free previous cache if any */
    if (primeCache) free(primeCache);
    build_primes_upto(limit, &primeCache, &primeCount);
    maxPrimeLimit = limit;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;                    /* no defined term before offset */

    /* get the n-th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* Ensure we have all primes up to p in our cache */
    ensure_primes_up_to(p);

    /* Count suitable q < p */
    seq_t cnt = 0;
    size_t i;
    for (i = 0; i < primeCount; ++i) {
        uint64_t q = primeCache[i];
        if (q >= p) break;                   /* remaining primes are too large */
        uint64_t test = (uint64_t)2 * p + q;

        if (is_prime_small(test)) ++cnt;
    }

    return cnt;
}
