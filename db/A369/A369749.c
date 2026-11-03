/*
 * A369749  a(n) = Sum_{p|n, p prime} p * prime(n/p).
 * https://oeis.org/A369749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p * prime(n/p)
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
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

/* Compute a(n) for any n >= 1.
   prime(k) is obtained from get_prime, a 1‑based prime index. */
seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 0 by definition */
        return (seq_t)0;

    seq_t sum = 0;
    uint64_t m = (uint64_t)n;

    /* handle factor 2 separately */
    if ((m & 1ULL) == 0ULL) {
        uint64_t p = 2ULL;
        idx_t idx = n / p;                /* n/p is integer */
        sum += (seq_t)p * (seq_t)get_prime(idx);
        while ((m & 1ULL) == 0ULL)
            m >>= 1;                      /* divide out all 2's */
    }

    /* odd factors */
    for (uint64_t d = 3ULL; d * d <= m; d += 2ULL) {
        if (m % d == 0ULL) {
            uint64_t p = d;
            idx_t idx = n / p;
            sum += (seq_t)p * (seq_t)get_prime(idx);
            while (m % d == 0ULL)
                m /= d;                  /* divide out all copies of d */
        }
    }

    /* if a prime factor larger than sqrt remains */
    if (m > 1ULL) {
        uint64_t p = m;
        idx_t idx = n / p;
        sum += (seq_t)p * (seq_t)get_prime(idx);
    }

    return sum;
}
