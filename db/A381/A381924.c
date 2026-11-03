/*
 * A381924  Multiplicative order of n mod prime(n).
 * https://oeis.org/A381924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of n modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 3500, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Validated: standard, pass, 81 terms through n = 3456 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

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

/* Computes the least k such that prime(n) divides n^k – 1.
   prime(n) is obtained via get_prime(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for unexpected calls */

    /* Obtain the n-th prime (as uint32_t) and promote to 64‑bit. */
    uint32_t p_u32 = get_prime(n);
    uint64_t p     = (uint64_t)p_u32; 

    /* Factor p – 1 into distinct prime factors. */
    uint64_t m = p - 1;
    uint64_t fac[64];
    size_t fac_cnt = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            fac[fac_cnt++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) fac[fac_cnt++] = m;

    /* Compute multiplicative order. */
    uint64_t order = p - 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = fac[i];
        while (order % q == 0) {
            uint64_t exp = order / q;
            if (powmod((seq_t)n, exp, (seq_t)p) == (seq_t)1)
                order = exp;
            else
                break;
        }
    }

    return (seq_t)order;
}
