/*
 * A276086  Primorial base exp-function: digits in primorial base
 *          representation of n become the exponents of successive prime
 *          factors whose product a(n) is.
 * https://oeis.org/A276086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∏_{k≥0} prime(k+1)^{d_k(n)}, where d_k(n) are the digits
 *            of n written in primorial base. a(0) = 1
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 416, u64 n <= 2308,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 90 terms through n = 2304 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ipow.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                       /* a(0) = 1 */

    /* a small fixed buffer is sufficient because n is small;
       primorials grow very fast. */
    const int MAX_K = 64;
    uint64_t prim[MAX_K];
    prim[0] = 1;                         /* 0‑th primorial = 1 */
    uint64_t max_k = 0;

    /* Build primorials p_k = product of first k+1 primes
       until the next would exceed the index n. */
    for (uint64_t k = 1; k < MAX_K; ++k) {
        uint64_t p = get_prime(k);        /* prime(k+1)   */
        if (prim[k-1] > UINT64_MAX / p)   /* prevent 64‑bit overflow */
            break;
        prim[k] = prim[k-1] * p;
        if (prim[k] > n)
            break;                       /* this primorial too large */
        max_k = k;                       /* record the largest valid k */
    }

    seq_t result = 1;
    uint64_t rem   = n;

    /* Iterate from most significant digit down to the least. */
    for (int k = (int)max_k; k >= 0; --k) {
        uint64_t digit = rem / prim[k];
        rem -= digit * prim[k];

        if (digit == 0)
            continue;                    /* multiplying by 1 is useless */

        seq_t base = (seq_t)get_prime(k + 1); /* prime(k+1)  */
        seq_t pw   = ipow(base, (seq_t)digit);
        result = seq_mul(result, pw);
    }

    return result;
}
