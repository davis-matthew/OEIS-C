/*
 * A271811  Number of non-abelian groups of order prime(n)^6.
 * https://oeis.org/A271811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non-abelian groups of order prime(n)^6
 * Range:     u8 none, u16 n <= 34, u32 n <= 1229, u64 n <= 1229,
 *            u128 n <= 1229, big any n
 * Validated: standard, pass, 59 terms through n = 1120 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the explicit closed form for p > 3, with
 * special cases for p = 2 and p = 3. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* obtain the n-th prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* special cases for p = 2 and p = 3 */
    if (p32 == 2) return (seq_t)256;        /* 267 total groups – 11 abelian */
    if (p32 == 3) return (seq_t)493;        /* 504 total groups – 11 abelian */

    /* p > 3 : use the explicit formula  
       a(p) = 3p^2 + 39p + 333 + 24 gcd(p-1,3) +
              11 gcd(p-1,4) + 2 gcd(p-1,5)   */
    seq_t p_minus1 = p - 1;

    seq_t result = 3 * p * p;                    /* 3p^2 */
    result += 39 * p;                            /* +39p */
    result += (seq_t)333;                        /* +333 */

    result += 24 * gcd(p_minus1, (seq_t)3);      /* +24 gcd(p-1,3) */
    result += 11 * gcd(p_minus1, (seq_t)4);      /* +11 gcd(p-1,4) */
    result += 2  * gcd(p_minus1, (seq_t)5);      /* +2  gcd(p-1,5) */

    return result;
}
