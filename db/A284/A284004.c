/*
 * A284004  a(n) = A046523(A284003(n)).
 * https://oeis.org/A284004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A284003(n)) * A284003(n) is defined recursively
 *            by repeatedly setting * n = n-p + p/2 or n-p - p/2 where p is
 *            the largest power of 2 ≤ n. * A046523 is the primorial (product
 *            of the first k primes). * Thus a(n) is the product of the first
 *            k(n) primes
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 511, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 96 terms through n = 7759 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static idx_t _a284003(idx_t n)
{
    if (n == 0) return 0;
    idx_t k = 0;
    while (n) {
        idx_t msb = 63;
        while ((n >> msb) == 0) msb--;           /* highest set bit */
        idx_t p = ((idx_t)1) << msb;             /* 2^msb ≤ n */
        idx_t half = p >> 1;                     /* p/2 */
        idx_t offset = n - p;
        idx_t j = (offset < half) ? (offset + half) : (offset - half);
        n = j;
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    idx_t k = _a284003(n);
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)get_prime(i);
    }
    return res;
}
