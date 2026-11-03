/*
 * A246489  Duodecimal period of 1/(n-th prime) (0 by convention for the
 *          primes 2 and 3).
 * https://oeis.org/A246489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative order of 12 modulo the n-th prime, with 0
 *            returned for the primes 2 and 3 (OEIS convention)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 7901 (2026-11-01)
 * Helpers:   factorize, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factorize.h"
#include "../utils/powmod.h"

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

seq_t a(idx_t n) {
    /* retrieve the n-th prime; the function returns it as a 32‑bit unsigned integer */
    uint64_t prime = (uint64_t)get_prime(n);

    /* by definition a(1)=0 for prime 2, and a(2)=0 for prime 3 */
    if (prime <= 3) {
        return (seq_t)0;
    }

    /* the multiplicative order of 12 modulo prime */
    uint64_t order = prime - 1;          /* φ(prime) = prime-1 */
    uint64_t prime_factors[64];
    int exp_factors[64];
    size_t pf_cnt = factorize(prime - 1, prime_factors, exp_factors);

    /* reduce the order by each prime factor in the factorisation of (prime-1) */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t q = prime_factors[i];
        while (order % q == 0 &&
               powmod((seq_t)12, order / q, (seq_t)prime) == (seq_t)1) {
            order /= q;
        }
    }

    return (seq_t)order;
}
