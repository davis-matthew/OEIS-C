/*
 * A297493  a(n) = (1/2) * Sum_{|k|<=2*sqrt(p)} k^8*H(4*p-k^2) where H() is
 *          the Hurwitz class number and p is n-th prime.
 * https://oeis.org/A297493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*prime(n)^5 - 28*prime(n)^3 - 20*prime(n)^2 -
 *            7*prime(n) - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 575, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 888 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here prime(n) is the nth prime, 1‑based */

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
    /* n ≥ 1: obtain the nth prime */
    uint32_t p = get_prime(n);
    seq_t x = (seq_t)p;

    /* Compute powers of x in seq_t */
    seq_t x2 = x * x;       /* x^2 */
    seq_t x3 = x2 * x;      /* x^3 */
    seq_t x5 = x3 * x2;     /* x^5 = x^3 * x^2 */

    /* Polynomial b(x) = 14*x^5 - 28*x^3 - 20*x^2 - 7*x - 1 */
    seq_t res = (seq_t)14 * x5;
    res -= (seq_t)28 * x3;
    res -= (seq_t)20 * x2;
    res -= (seq_t)7 * x;
    res -= (seq_t)1;

    return res;
}
