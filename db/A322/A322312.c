/*
 * A322312  a(n) = Product_{d|n, d+1 is prime} prime(1+A286561(n,d+1)), where
 *          A286561(n,k) gives the k-valuation of n (for k > 1).
 * https://oeis.org/A322312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d+1 is prime} prime(1 + A286561(n, d+1))
 * Range:     u8 n <= 35, u16 n <= 359, u32 n <= 7559, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 105 terms through n = 9709 (2026-11-01)
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Explicitly:  a(n) = ∏_{p prime, (p‑1) | n} prime(1 + v_p(n)) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

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
    /* static primes cache – rebuild if n+1 exceeds current limit */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n + 1 > prime_limit) {
        if (prime_list) free(prime_list);
        build_primes_upto(n + 1, &prime_list, &prime_cnt);
        prime_limit = n + 1;
    }

    /* factorization of n */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t fcnt = 0;
    factor_u64(n, factors, exps, &fcnt);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p   = prime_list[i];
        uint64_t step = p - 1;            /* d = p-1 */
        if (step == 0) continue;          /* p = 1 never occurs */
        if (n % step == 0) {              /* (p‑1) | n  */
            /* get v_p(n) from factorization (0 if not present) */
            uint64_t e = 0;
            for (size_t j = 0; j < fcnt; ++j) {
                if (factors[j] == p) { e = exps[j]; break; }
            }
            /* multiply by the (1+e)-th prime */
            idx_t idx = 1 + e;                    /* very small, fits in idx_t */
            seq_t prime_val = (seq_t)get_prime(idx);
            result *= prime_val;
        }
    }
    return result;
}
