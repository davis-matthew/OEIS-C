/*
 * A249490  a(n) = Sum_{m=0..floor((n-1)/2)} prime((n-m)(n-m-1)/2+m+1).
 * https://oeis.org/A249490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=0..floor((n-1)/2)} prime((n-m)(n-m-1)/2 + m + 1)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 903 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) retrieving the prime by index using the provided
   get_prime utility, so it works for any seq_t and idx_t defined by
   the build environment. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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
    if (n == 0) return 0;                 /* offset 1: undefined below, return 0 */

    idx_t m_max = (n - 1) / 2;             /* floor((n-1)/2) */
    seq_t sum  = 0;

    for (idx_t m = 0; m <= m_max; ++m) {
        idx_t t   = n - m;
        idx_t pos = (t * (t - 1)) / 2 + m + 1;   /* 1‑based prime index */
        sum += (seq_t)get_prime(pos);             /* get_prime returns uint32_t */
    }
    return sum;
}
