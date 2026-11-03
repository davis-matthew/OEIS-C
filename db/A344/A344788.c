/*
 * A344788  a(n) is the number of pairs of primes (q,r) with prime(n) > q > r
 *          such that q | r + prime(n) and r | q + prime(n).
 * https://oeis.org/A344788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9824 (2026-11-01)
 * Helpers:   build_primes_upto, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) counts the number of pairs of primes (q,r) with
   prime(n) > q > r such that q divides r + prime(n) and r divides q + prime(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* prime(n) */
    uint64_t p = get_prime(n);

    /* List of primes strictly less than p */
    uint64_t *primes = NULL;
    size_t plen = 0;
    if (p > 2) {
        build_primes_upto(p - 1, &primes, &plen);
    }

    seq_t count = 0;

    for (size_t i = 0; i < plen; ++i) {
        uint64_t q = primes[i];

        /* Candidate r that satisfies q | r + p */
        uint64_t r = q - (p % q);

        /* r must be a proper prime < q */
        if (r >= q || r <= 1) continue;
        if (!is_prime_seq(r)) continue;

        /* Verify the reciprocal divisibility condition r | q + p */
        if ((q + p) % r != 0) continue;

        ++count;
    }

    free(primes);
    return count;
}
