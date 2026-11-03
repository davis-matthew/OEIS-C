/*
 * A306995  Number of primes q < prime(n), such that 2*prime(n) + q is prime.
 * https://oeis.org/A306995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q < prime(n) with 2*prime(n)+q prime
 * Range:     u8 n <= 24, u16 n <= 3259, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 95 terms through n = 7491 (2026-11-01)
 * Helpers:   build_primes_upto, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                /* malloc, free */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_seq.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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

        if (is_prime_seq(test)) ++cnt;
    }

    return cnt;
}
