/*
 * A339640  a(n) = (A062772(n) + A054270(n)) / 2 - A001248(n).
 * https://oeis.org/A339640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (nextprime(prime(n)^2) + precprime(prime(n)^2))/2 -
 *            prime(n)^2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9855 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

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

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; safe guard */

    uint64_t p   = (uint64_t)get_prime(n);      /* nth prime, fits in 32‑bit */
    uint64_t p2  = p * p;                      /* p^2, fits in 64‑bit */

    uint64_t prev = prev_prime(p2);            /* previous prime < p^2 */
    uint64_t next = next_prime(p2);            /* next prime   > p^2 */

    /* use 64‑bit intermediates to avoid overflow when seq_t is smaller */
    int64_t avg = ((int64_t)prev + (int64_t)next) / 2;
    int64_t res = avg - (int64_t)p2;

    return (seq_t)res;
}
