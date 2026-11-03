/*
 * A271227  Number of solutions to y^2 == x^3 + 17 (mod p) as p runs through
 *          the primes.
 * https://oeis.org/A271227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6498, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 7138 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n) = number of solutions to y^2 = x^3 + 17 (mod prime(n)) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* ----------------- caching ------------------------------------------ */
    static seq_t *cache = NULL;      /* 1‑based: cache[n-1] holds a(n)          */
    static idx_t cached  = 0;        /* number of entries cached so far        */

    if (n <= cached && cache[n - 1] != 0)
        return cache[n - 1];

    /* ----------------- prime of index n -------------------------------- */
    uint32_t p = (uint32_t)get_prime(n);    /* p fits into 32 bits               */

    /* ----------------- compute number of solutions --------------------- */
    seq_t count;

    /* Silverman theorem */
    if (p == 2) {
        count = (seq_t)p;
    } else if (p == 3) {
        count = (seq_t)p;        /* 3 solutions for p = 3 */
    } else if (p % 3U == 2U) {   /* p ≡ 2 (mod 3)         */
        count = (seq_t)p;
    } else {                     /* p ≡ 1 (mod 3)         */
        count = 0;
        for (uint32_t x = 0; x < p; ++x) {
            uint64_t t = (uint64_t)x * x;          /* x^2  */
            t %= p;
            t = (t * x) % p;                        /* x^3  */
            t += 17;
            if (t >= p) t -= p;
            uint64_t v = t;                         /* 0 ≤ v < p    */

            if (v == 0) {
                count += 1;                        /* y = 0 only    */
            } else {
                int leg = jacobi_symbol(v, p);      /* Legendre symbol */
                if (leg == 1)
                    count += 2;                    /* two square roots */
            }
        }
    }

    /* ----------------- store in cache --------------------------------- */
    if (n > cached) {
        /* realloc may return void*; cast to seq_t* for C++ */
        cache = mem_resize(seq_t, cache, n);
        for (idx_t i = cached; i < n; ++i)
            cache[i] = 0;
        cached = n;
    }
    cache[n - 1] = count;

    return count;
}
