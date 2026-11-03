/*
 * A294876  a(n) = Product_{d|n, d>1} prime(gcd(d,n/d)).
 * https://oeis.org/A294876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d|n , d>1} prime(gcd(d , n/d)) (OEIS A294876)
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 143, u64 n <= 719,
 *            u128 n <= 3599, big any n
 * Validated: standard, pass, 108 terms through n = 9839 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n == 1)            /* a(1) = 1 */
        return (seq_t)1;

    seq_t prod = (seq_t)1;

    /* Enumerate divisors d from 2 up to sqrt(n) without overflow */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        idx_t e = n / d;                         /* complementary divisor */

        /* gcd(d, e) fits in idx_t because both ≤ n */
        seq_t g = gcd((seq_t)d, (seq_t)e);
        idx_t k = (idx_t)g;                      /* safe cast, g ≤ n ≤ UINT64_MAX */

        seq_t p = (seq_t)get_prime(k);           /* prime(k) */

        prod *= p;                               /* divisor d */

        if (e != d)                              /* divisor e (if distinct) */
            prod *= p;
    }

    /* Handle the divisor d = n (paired with 1): gcd(n,1) = 1, prime(1)=2 */
    prod *= (seq_t)2;

    return prod;
}
