/*
 * A280871  Denominator of the mediant of prime(n) / prime(n+1) and prime(n+2)
 *          / prime(n+3).
 * https://oeis.org/A280871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (prime(n+1)+prime(n+3)) / gcd(prime(n)+prime(n+2),
 *            prime(n+1)+prime(n+3))
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 975 (2026-11-01)
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
    if (n < 1) {
        return 0;
    }

    /* Retrieve the four consecutive primes needed */
    uint64_t p0 = (uint64_t)get_prime(n);
    uint64_t p1 = (uint64_t)get_prime(n + 1);
    uint64_t p2 = (uint64_t)get_prime(n + 2);
    uint64_t p3 = (uint64_t)get_prime(n + 3);

    /* Numerator and denominator of the mediant */
    uint64_t num = p0 + p2;
    uint64_t den = p1 + p3;

    /* Reduce the fraction: denominator / gcd(numerator, denominator) */
    seq_t g   = gcd((seq_t)num, (seq_t)den);
    uint64_t g64 = (uint64_t)g;
    uint64_t res64 = den / g64;

    return (seq_t)res64;
}
