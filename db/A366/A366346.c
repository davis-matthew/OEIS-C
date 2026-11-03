/*
 * A366346  a(n) = A002070(n) + A036689(n).
 * https://oeis.org/A366346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A366346
 *  a(n) = A002070(n) + A036689(n)
 *
 *  Equivalent formula (valid for n ≥ 1):
 *      p = prime(n)
 *      a(n) = p^2 - p - Σ_{x=0}^{p-1} χ(1 + 4·(x^3 - x^2) mod p)
 *      where χ(·) is the Legendre symbol modulo the odd prime p.
 *
 *  In the special case p = 2 the value is 0.
 *
 *  The function below implements this formula and works for all
 *  supported `seq_t` and `idx_t` types.  It is compatible with
 *  C11 and C++17 and uses only the utilities listed in the
 *  problem statement.
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

/* Legendre symbol (a|p) for odd prime p. Returns -1, 0, or 1. */
static inline int8_t legendre_modp(uint64_t a, uint64_t p)
{
    a %= p;
    if (a == 0) return 0;
    uint64_t exp = (p - 1) >> 1;
    uint64_t e = powmod_u64(a, exp, p);
    if (e == p - 1) return -1;
    return 1;          /* e == 1 */
}

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑indexed). */
    uint64_t p = (uint64_t)get_prime(n);

    /* Handle the only exceptional prime (p = 2). */
    if (p == 2) return (seq_t)0;

    /* Sum of Legendre symbols over all residues x. */
    int64_t sum_leg = 0;
    for (uint64_t x = 0; x < p; ++x) {
        uint64_t x2 = (x * x) % p;
        uint64_t x3 = (x2 * x) % p;
        uint64_t diff = (x3 + p - x2) % p;          /* x^3 - x^2 mod p */
        uint64_t d    = (1 + (4 * diff) % p) % p;   /* 1 + 4·(x^3 - x^2) mod p */
        sum_leg += (int64_t)legendre_modp(d, p);
    }

    /* a(p) = p^2 - p - sum_leg. */
    int64_t ans_int = (int64_t)p * (int64_t)p - (int64_t)p - sum_leg;
    return (seq_t)ans_int;
}
