/*
 * A364937  a(n) is the least prime p such that p - prime(n) and p + prime(n)
 *          are triprimes.
 * https://oeis.org/A364937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least prime p such that p – prime(n) and p +
 *            prime(n) * are triprimes (products of exactly three primes
 *            counting multiplicity)
 * Range:     u8 n <= 38, u16 n <= 6301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9373 (2026-11-01)
 * Helpers:   build_primes_upto, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"

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

/* Test whether m is a product of exactly three primes (with multiplicity). */
static bool is_triprime(uint64_t m, const uint64_t *primes, size_t primes_cnt)
{
    if (m < 8)         /* 2³ = 8 is the smallest triprime */
        return false;

    size_t cnt = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        while (m % p == 0) {
            ++cnt;
            if (cnt > 3)
                return false;
            m /= p;
        }
    }
    if (m > 1) {             /* remaining factor is a prime larger than sqrt */
        ++cnt;
        if (cnt > 3)
            return false;
    }
    return cnt == 3;
}

/* Forward declaration of the triprime tester */
static bool is_triprime(uint64_t m, const uint64_t *primes, size_t primes_cnt);

/* Public entry point required by the build system */
seq_t a(idx_t n)
{
    /* lazy static construction of a prime table for factorisation */
    static bool init = false;
    static uint64_t *factor_primes = NULL;
    static size_t factor_cnt = 0;

    if (!init) {
        /* 200 000 is comfortably larger than any prime needed for n ≤ 10 000 */
        const idx_t FP_LIMIT = 200000;
        build_primes_upto(FP_LIMIT, &factor_primes, &factor_cnt);
        init = true;
    }

    /* nth prime (OEIS indexing starts at 1) */
    uint64_t q = (uint64_t)get_prime(n);

    /* search for the smallest prime p > q satisfying the conditions */
    uint64_t p = next_prime(q);
    while (true) {
        uint64_t m1 = p - q;
        uint64_t m2 = p + q;

        if (is_triprime(m1, factor_primes, factor_cnt) &&
            is_triprime(m2, factor_primes, factor_cnt))
            return (seq_t)p;

        p = next_prime(p);
    }
}
