/*
 * A396240  Denominator of r(n) = (prime(n+2) - 2*prime(n+1) + prime(n)) /
 *          (prime(n+2) - prime(n)).
 * https://oeis.org/A396240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of (prime(n+2) - 2*prime(n+1) + prime(n)) /
 *            (prime(n+2) - prime(n)) for n >= 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9868 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

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

/* Compute the n-th term, with offset 1 (a(1) is the first term). */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. The behaviour for n == 0 is undefined. */
    if (n == 0) {
        return 0;
    }

    /* Retrieve the three consecutive primes needed. */
    uint32_t p1 = get_prime(n);       /* prime(n)   */
    uint32_t p2 = get_prime(n + 1);   /* prime(n+1) */
    uint32_t p3 = get_prime(n + 2);   /* prime(n+2) */

    /* Compute the absolute value of the numerator. */
    int64_t num_signed = (int64_t)p3 - 2LL * (int64_t)p2 + (int64_t)p1;
    uint64_t num = (num_signed >= 0) ? (uint64_t)num_signed : (uint64_t)-num_signed;

    /* Compute the denominator (always positive). */
    uint64_t denom = (uint64_t)p3 - (uint64_t)p1;

    /* Reduce the fraction and return the denominator. */
    uint64_t g = gcd_u64(num, denom);
    return (seq_t)(denom / g);
}
