/*
 * A335302  a(n) = prime(n+1) mod (2^k) where k is the least positive integer
 *          such that floor(prime(n)/(2^k)) = floor(prime(n+1)/(2^k)) and
 *          prime(n) denotes the n-th prime number.
 * https://oeis.org/A335302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 8312 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335302: a(n) = prime(n+1) mod 2^k where k is the least positive integer
 * such that floor(prime(n)/2^k) = floor(prime(n+1)/2^k).
 * (Equivalently, k is one more than the position of the highest differing
 * bit between consecutive primes.)
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

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
    /* n is 1‑based, we need primes n and n+1 */
    seq_t p1 = (seq_t)get_prime((idx_t)n);      /* prime(n)   */
    seq_t p2 = (seq_t)get_prime((idx_t)n + 1);  /* prime(n+1) */

    /* find smallest k ≥ 1 with (p1 >> k) == (p2 >> k) */
    idx_t k = 1;
    while (((p1 >> k) != (p2 >> k))) {
        ++k;
    }

    /* mask for the lowest k bits of p2 */
    seq_t mask = ((seq_t)1 << k) - 1;
    return p2 & mask;
}
