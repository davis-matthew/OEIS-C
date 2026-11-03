/*
 * A396938  a(1) = 1, a(n) = Product_{i=1..omega(n)} prime(i)^(m_i-1), with n
 *          = Product_{i=1..omega(n)} p_i^m_i, omega = A001221, and p_i <
 *          p_(i+1).
 * https://oeis.org/A396938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..ω(n)} prime(i)^(m_i-1) where * n = ∏ p_i^{m_i}
 *            (p_i sorted ascending)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8743 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Utilities for factorization, exponentiation and primes */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

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
    /* n = 1 is a special case */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* n fits in 64 bits; factor_u64 works on uint64_t */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];   /* primes dividing n */
    uint64_t exps[64];     /* their exponents */
    size_t   cnt = 0;      /* number of distinct prime factors */

    /* Factorise nn, storing primes and exponents */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    /* For each distinct prime factor, use the i‑th prime (2,3,5,…) as the
       base.  The exponent is (m_i-1); skip if it is zero. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t m = exps[i];
        if (m <= 1) {          /* m-1 == 0 → factor 1 */
            continue;
        }
        uint64_t base = (uint64_t)get_prime((idx_t)(i + 1));   /* kth prime */
        seq_t    power = ipow_u64_idx(base, (idx_t)(m - 1));
        result *= power;
    }

    return result;
}
