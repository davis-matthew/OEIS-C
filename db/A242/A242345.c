/*
 * A242345  Number of primes p < prime(n) with p and 2^p - p both primitive
 *          roots modulo prime(n).
 * https://oeis.org/A242345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 3500, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Validated: standard, pass, 96 terms through n = 3379 (2026-11-01)
 * Helpers:   build_primes_upto, factorize, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = number of primes p < prime(n) such that both p and
   2^p - p are primitive roots modulo prime(n).  The implementation
   uses a small sieve for the primes < prime(n) and simple lifting of
   the primitive‑root test. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <string.h>
#include "../utils/build_primes_upto.h"
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

seq_t a(idx_t n)
{
    /* offset is 1 as in OEIS; return 0 for indices < 1 to be safe */
    if (n == 0) return 0;

    /* prime(n) only fits in 32 bits for n ≤ 10000 */
    uint32_t P = get_prime(n);
    if (P <= 2) return 0;   /* no prime < 2 */

    /* gather all primes < P */
    uint64_t *prime_list = NULL;
    size_t prime_cnt   = 0;
    build_primes_upto((idx_t)P - 1, &prime_list, &prime_cnt);

    /* factorise P‑1 to collect its distinct prime divisors */
    uint64_t pf[64];
    int      exp[64];      /* exponents not needed */
    size_t pf_cnt = factorize((uint64_t)P - 1, pf, exp);

    seq_t count = 0;

    /* iterate over primes < P */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];   /* candidate prime */

        /* check that q is a primitive root modulo P */
        bool ok = true;
        for (size_t j = 0; j < pf_cnt && ok; ++j) {
            uint64_t e = ((uint64_t)P - 1) / pf[j];
            if (powmod((seq_t)q, e, (seq_t)P) == 1) ok = false;
        }
        if (!ok) continue;

        /* compute (2^q – q) mod P */
        seq_t pow2 = powmod((seq_t)2, q, (seq_t)P);
        seq_t t = pow2 + (seq_t)P - (seq_t)q;   /* q < P, safe cast */
        if (t >= (seq_t)P) t -= (seq_t)P;

        if (t == (seq_t)0) continue;   /* 0 cannot be a primitive root */

        /* check that t is a primitive root modulo P */
        for (size_t j = 0; j < pf_cnt && ok; ++j) {
            uint64_t e = ((uint64_t)P - 1) / pf[j];
            if (powmod(t, e, (seq_t)P) == 1) ok = false;
        }
        if (ok) ++count;
    }

    free(prime_list);
    return count;
}
