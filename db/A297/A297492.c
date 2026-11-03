/*
 * A297492  a(n) = (1/2) * Sum_{|k|<=2*sqrt(p)} k^6*H(4*p-k^2) where H() is
 *          the Hurwitz class number and p is the n-th prime.
 * https://oeis.org/A297492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*prime(n)^4 – 9*prime(n)^2 – 5*prime(n) – 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 44 terms through n = 982 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* first get the n-th prime – it comes as an unsigned 32‑bit int */
    seq_t p = (seq_t)get_prime(n);

    /* compute p^2 and p^4 using only seq_t arithmetic */
    seq_t p2 = p * p;
    seq_t p4 = p2 * p2;

    /* compute the polynomial 5p^4 – 9p^2 – 5p – 1 */
    seq_t val = 5 * p4;
    val -= 9 * p2;
    val -= 5 * p;
    val -= 1;

    return val;
}
