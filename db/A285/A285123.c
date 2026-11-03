/*
 * A285123  Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1),..,d(k) are the
 *          divisors of prime(n) - 1.
 * https://oeis.org/A285123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(|d(k+1-i) - d(i)| for i = 1..k), * where d(1..k) are
 *            the divisors of prime(n) - 1. * Equivalent: a(n) = min_{d |
 *            (p-1)} | d - (p-1)/d |, p = prime(n)
 * Range:     u8 n <= 55, u16 n <= 8726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9782 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

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
    uint32_t p = get_prime(n);          /* p is the n-th prime */
    uint64_t m = (uint64_t)p - 1ULL;      /* m = prime(n) - 1 */

    /* Compute floor(sqrt(m)) without floating point */
    uint64_t sqrtN = 0;
    for (sqrtN = 1; (sqrtN + 1) * (sqrtN + 1) <= m; ++sqrtN) { }

    seq_t minDiff = (seq_t)m;           /* diff never exceeds m-1 */

    for (uint64_t d = 1; d <= sqrtN; ++d) {
        if (m % d == 0) {
            uint64_t c = m / d;
            uint64_t diff = (d > c) ? d - c : c - d;
            if (diff < (uint64_t)minDiff) {
                minDiff = (seq_t)diff;
                if (minDiff == (seq_t)0) break;   /* cannot be smaller */
            }
        }
    }
    return minDiff;
}
