/*
 * A356489  a(n) = A000265(rad(prime(n)-1)), rad = A007947.
 * https://oeis.org/A356489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{odd primes p dividing prime(n)-1} p, for n ≥ 1
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8343 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

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
    if (n == 0)                   /* undefined for offset < 1, return 0 as placeholder */
        return (seq_t)0;

    /* Fetch the n‑th prime (1‑based: get_prime(1) == 2). */
    uint32_t p = get_prime(n);
    uint64_t m = (uint64_t)p - 1ULL;       /* m = prime(n) - 1 */

    /* Factor m into distinct primes. */
    uint64_t fac[64];                   /* prime factors */
    uint64_t exps[64];                  /* exponents (unused) */
    size_t   cnt = 0;                   /* number of distinct factors */

    factor_u64(m, fac, exps, &cnt);

    /* Multiply all odd primes found. */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (fac[i] != 2ULL)
            prod *= (seq_t)fac[i];
    }
    return prod;
}
