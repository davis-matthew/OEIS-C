/*
 * A275630  a(n) = product of distinct primes dividing prime(n)^2 - 1.
 * https://oeis.org/A275630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of distinct primes dividing prime(n)^2 - 1
 * Range:     u8 n <= 13, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9575 (2026-11-01)
 * Helpers:   build_primes_upto, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"

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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, but safe return */

    uint32_t p = get_prime(n);            /* nth prime */
    uint64_t m = (uint64_t)p * p - 1;     /* p^2 - 1 */

    /* static cache for primes up to the needed limit */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static uint64_t prime_max = 0;

    uint64_t limit = isqrt((idx_t)m);  /* sqrt(m) needed for factorisation */

    if (limit > prime_max) {
        /* free old list if any */
        if (primes) free(primes);
        /* build a new list of all primes up to limit */
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_max = limit;
    } else if (!primes) {                /* first call when limit <= 0 */
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_max = limit;
    }

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = primes[i];
        if (q * q > m) break;
        if (m % q == 0) {
            result *= (seq_t)q;
            while (m % q == 0) m /= q;
            if (m == 1) break;
        }
    }

    if (m > 1)                     /* remaining prime factor (> sqrt) */
        result *= (seq_t)m;

    return result;
}
