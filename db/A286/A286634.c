/*
 * A286634  Numerator of the ratio of alternate consecutive prime gaps:
 *          Numerator ((prime(n + 3) - prime(n + 2))/(prime(n + 1) -
 *          prime(n))).
 * https://oeis.org/A286634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((prime(n+3)-prime(n+2))/(prime(n+1)-prime(n)))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerator of the reduced fraction formed by the ratio of */
/* two consecutive prime gaps, where the gaps are taken for primes index */
/* n, n+1, n+2, n+3. */

#include <stdint.h>
#include <stdbool.h>
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
    /* Retrieve the required primes.
       get_prime(k) returns the k-th prime (1‑based). */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 2);
    uint32_t p4 = get_prime(n + 3);

    /* Compute the two prime gaps. */
    seq_t g1 = (seq_t)p2 - (seq_t)p1; /* gap between prime n and prime n+1 */
    seq_t g2 = (seq_t)p4 - (seq_t)p3; /* gap between prime n+2 and prime n+3 */

    /* Reduce the fraction g2/g1 to lowest terms. */
    seq_t d = gcd(g1, g2);
    return g2 / d;
}
