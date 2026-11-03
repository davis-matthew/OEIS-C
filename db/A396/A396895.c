/*
 * A396895  a(n) = 4*p - floor(2*sqrt(p))^2, where p = prime(n).
 * https://oeis.org/A396895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*prime(n) - floor(2*sqrt(prime(n)))^2
 * Range:     u8 n <= 588, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 8381 (2026-11-01)
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

/* Integer square‑root for unsigned 64‑bit values.
   Returns floor(sqrt(x)).  Works for the modest limits of this
   sequence (p ≤ 104 729).  */
static inline unsigned long long isqrt_ull(unsigned long long x)
{
    unsigned long long lo = 0, hi = x, mid, res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        unsigned long long sq = mid * mid;
        if (sq == x) {
            return mid;
        }
        if (sq < x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid infinite loop when x < 1 */
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* get the n‑th prime, zero‑based internal but OEIS offset 1 */
    uint32_t p32 = get_prime(n);
    unsigned long long p = (unsigned long long)p32;   /* intermediate value */

    unsigned long long fourp = p * 4ULL;              /* 4*p */
    unsigned long long t = isqrt_ull(fourp);         /* floor(2*sqrt(p)) */
    unsigned long long ans = fourp - t * t;          /* a(n) */

    return (seq_t)ans;
}
