/*
 * A305331  Multiplicative order of 5 (mod p^2), where p = prime(n), or 0 if 5
 *          and p are not coprime.
 * https://oeis.org/A305331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 5 modulo p^2, with p = prime(n);
 *            returns 0 if p = 5 (since 5 and p are not coprime)
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/powmod_u64.h"

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
    if (n < 1) return 0;               /* offset handling */
    uint32_t p = get_prime(n);          /* p = prime(n) */

    if (p == 5) return 0;               /* 5 and p not coprime */

    uint64_t mod  = (uint64_t)p * p;    /* p^2 */
    uint64_t ord  = (uint64_t)p * (p - 1); /* φ(p^2) = p(p-1) */

    /* First handle prime factors of p-1 */
    uint64_t t = p - 1;
    for (uint64_t d = 2; d * d <= t; ++d) {
        if (t % d == 0) {
            /* count multiplicity of d in (p-1) */
            int cnt = 0;
            while (t % d == 0) { t /= d; cnt++; }
            /* attempt to divide ord by d as many times as possible */
            for (int i = 0; i < cnt; ++i) {
                if (ord % d == 0) {
                    uint64_t cand = ord / d;
                    if (powmod_u64(5, cand, mod) == 1)
                        ord = cand;
                    else
                        break;
                }
            }
        }
    }
    if (t > 1) { /* t is prime */
        uint64_t d = t;
        if (ord % d == 0) {
            uint64_t cand = ord / d;
            if (powmod_u64(5, cand, mod) == 1)
                ord = cand;
        }
    }

    /* Finally try to divide by p itself (exponent 1 in φ) */
    if (ord % p == 0) {
        uint64_t cand = ord / p;
        if (powmod_u64(5, cand, mod) == 1)
            ord = cand;
    }

    return (seq_t)ord;
}
