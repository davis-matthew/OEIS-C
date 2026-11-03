/*
 * A346470  a(n) = psi(A276086(n)), where psi is Dedekind psi function
 *          A001615, and A276086 is the prime product form of primorial base
 *          expansion of n.
 * https://oeis.org/A346470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = psi(A276086(n))
 * Range:     u8 n <= 14, u16 n <= 56, u32 n <= 388, u64 n <= 2302,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 80 terms through n = 2235 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* psi(n) = n * Π_{p|n} (1 + 1/p) */
/* A276086(n) is written as Π_i p_i^{d_i} where (d_i) are the digits of n
   in the mixed-radix system with bases 2,3,5,7,… (prime bases). */

/* The function below computes the Dedekind ψ value directly from the
   digits, avoiding an explicit factorisation of A276086(n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_idx.h"

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
    idx_t temp   = n;
    seq_t result = 1;

    for (idx_t i = 1; temp > 0; ++i) {
        uint32_t prime = get_prime(i);          /* i‑th prime (2,3,5,…) */
        idx_t    base  = (idx_t)prime;
        idx_t    digit = temp % base;

        temp /= base;                           /* shift to next digit */

        if (digit == 0)
            continue;                           /* prime not present */

        /* For digit > 0: contribute p^{digit-1} * (p+1) to ψ */
        seq_t term = ipow_idx((seq_t)prime, (idx_t)(digit - 1));
        term *= (seq_t)(prime + 1);
        result *= term;
    }

    return result;
}
