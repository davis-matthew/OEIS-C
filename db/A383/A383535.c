/*
 * A383535  Heinz number of the positive first differences of the 0-prepended
 *          prime indices of n.
 * https://oeis.org/A383535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Heinz number of the positive first differences of the *
 *            0‑prepended prime indices of n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

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
    if (n == (idx_t)1) return (seq_t)1;

    /* Build list of primes up to n (needed both for factorisation
     * and for retrieving prime indices). */
    idx_t *prime_tmp = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &prime_tmp, &prime_cnt);          /* primes are idx_t (uint64_t) */
    uint64_t *primes       = (uint64_t *)prime_tmp;        /* cast to uint64_t for arithmetic */

    seq_t result = (seq_t)1;       /* the Heinz number */
    uint64_t rem   = (uint64_t)n;   /* remaining part to factorise */
    idx_t prev_idx = 0;             /* previous prime index in the sequence   */

    /* Factorise n using the prime list.  For each prime factor p with
     * multiplicity e we process its index e times, updating the
     * "previous index" and multiplying by prime(diff) where diff is the
     * positive difference between consecutive indices. */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p   = primes[i];
        idx_t     pidx = (idx_t)(i + 1);     /* 1‑based prime index */

        if (p > rem / p) break;             /* p*p > rem  (avoid overflow) */

        if (rem % p == 0)
        {
            unsigned exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }

            for (unsigned k = 0; k < exp; ++k)
            {
                idx_t diff = pidx - prev_idx;   /* positive difference? */
                if (diff > (idx_t)0)
                    result *= (seq_t)get_prime(diff);
                prev_idx = pidx;
            }
        }
    }

    /* If a prime factor remains (rem > 1), it is the last one.  Find its
     * index by scanning the prime list and process it. */
    if (rem > (uint64_t)1)
    {
        idx_t r = (idx_t)rem;
        idx_t ridx = 0;                      /* 1‑based index of 'rem' */

        for (size_t i = 0; i < prime_cnt; ++i)
            if (primes[i] == r)
            {
                ridx = (idx_t)(i + 1);
                break;
            }

        idx_t diff = ridx - prev_idx;
        if (diff > (idx_t)0)
            result *= (seq_t)get_prime(diff);
        /* prev_idx = ridx;  // not needed afterwards */
    }

    free(primes);
    return result;
}
