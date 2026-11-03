/*
 * A319075  Square array T(n,k) read by antidiagonal upwards in which row n
 *          lists the n-th powers of primes, hence column k lists the powers
 *          of the k-th prime, n >= 0, k >= 1.
 * https://oeis.org/A319075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = prime(k)^(s-r) where s is the largest integer with
 *            s*(s+1)/2 <= n, k = (n - s*(s+1)/2) + 1 and exp = s - (n -
 *            s*(s+1)/2)
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_idx.h"

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

/* Find the largest s such that s*(s+1)/2 <= n */
static idx_t diag_index(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 <= n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Sequence A319075 */
seq_t a(idx_t n)
{
    idx_t s      = diag_index(n);                     /* diagonal number   */
    idx_t before = (s * (s + 1)) / 2;                 /* count before diag */
    idx_t r      = n - before;                        /* offset inside diag*/
    idx_t k      = r + 1;                              /* column (prime index) */
    idx_t exp    = s - r;                              /* row (exponent)      */

    uint32_t prime_val = get_prime(k);                /* k-th prime (1‑based)*/
    seq_t base = (seq_t)prime_val;
    return ipow_idx(base, exp);                         /* prime^exp          */
}
