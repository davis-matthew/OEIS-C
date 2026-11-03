/*
 * A340673  If n is of the form s^(2^e), where s is a squarefree number, and e
 *          >= 0, then a(n) is the (1+e)-th prime, otherwise a(n) = 1.
 * https://oeis.org/A340673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9757 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340673:
   a(n) = (1+e)-th prime if n = s^(2^e) with s squarefree and e ≥ 0,
   otherwise a(n) = 1.  a(1) = 1. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
    /* Handle the special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Factor n.  We need at most 64 distinct prime factors for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If n has no prime factors (should not happen for n>1), treat as 1. */
    if (cnt == 0)
        return (seq_t)1;

    /* All exponents must be equal. */
    uint64_t exp = exps[0];
    for (size_t i = 1; i < cnt; ++i)
    {
        if (exps[i] != exp)
            return (seq_t)1;
    }

    /* The common exponent must be a power of two. */
    if (exp == 0 || (exp & (exp - 1)) != 0)
        return (seq_t)1;

    /* Compute e such that exp = 2^e. */
    unsigned int e = 0;
    uint64_t tmp = exp;
    while (tmp > 1)
    {
        tmp >>= 1;
        ++e;
    }

    /* Return the (1+e)-th prime. */
    uint32_t p = get_prime((idx_t)(e + 1));
    return (seq_t)p;
}
