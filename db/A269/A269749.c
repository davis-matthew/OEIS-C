/*
 * A269749  a(n) = 3*p^2+39*p+344+24*gcd(p-1,3)+11*gcd(p-1,4)+2*gcd(p-1,5),
 *          where p = prime(n).
 * https://oeis.org/A269749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*p^2+39*p+344+24*gcd(p-1,3)+11*gcd(p-1,4)+2*gcd(p-1,5),
 *            where p = prime(n)
 * Range:     u8 none, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 998 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

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
    if (n < 1) return 0;              /* undefined for n < 1, return 0 */

    /* Obtain the nth prime.  get_prime returns a 32‑bit unsigned integer. */
    uint32_t p_u32 = get_prime(n);
    seq_t p          = (seq_t)p_u32;   /* cast to the required seq_t type */

    /* Compute the main part of the formula.  All arithmetic is performed
       in seq_t, which may truncate if the value does not fit. */
    seq_t term = (seq_t)3 * p * p;
    term += (seq_t)39 * p;
    term += (seq_t)344;

    /* Add the gcd terms. */
    seq_t gp1 = (seq_t)1;
    seq_t g3 = gcd(p - gp1, (seq_t)3);
    seq_t g4 = gcd(p - gp1, (seq_t)4);
    seq_t g5 = gcd(p - gp1, (seq_t)5);

    term += (seq_t)24 * g3;
    term += (seq_t)11 * g4;
    term += (seq_t) 2 * g5;

    return term;
}
