/*
 * A382185  a(n) is the n-th tribonacci number modulo the n-th prime.
 * https://oeis.org/A382185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = tribonacci(n) modulo the n‑th prime (OEIS A382185)
 * Range:     u8 n <= 61, u16 n <= 6585, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9858 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    /* n = 0 is outside the OEIS offset; return 0 safely */
    if (n == 0) return (seq_t)0;

    /* Retrieve the n‑th prime. get_prime returns a 32‑bit value. */
    uint32_t prime = get_prime(n);
    unsigned long long mod = (unsigned long long)prime;

    /* Tribonacci initial values (indices start at 1). */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    /* Compute T(n) modulo prime using a rolling window; no large
       intermediate values are needed, all arithmetic fits in 64 bits. */
    unsigned long long t1 = 0, t2 = 1, t3 = 1;   /* T(1), T(2), T(3) */
    for (idx_t i = 4; i <= n; ++i) {
        unsigned long long next = (t1 + t2 + t3) % mod;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }

    return (seq_t)t3;
}
