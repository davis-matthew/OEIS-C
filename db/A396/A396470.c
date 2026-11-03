/*
 * A396470  Sum of the distinct prime divisors of C(prime(n), n).
 * https://oeis.org/A396470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of C(prime(n), n)
 * Range:     u8 n <= 15, u16 n <= 227, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 892 (2026-11-01)
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

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
    /* p is the n‑th prime (p = prime(n)) */
    idx_t p = get_prime(n);           /* n ≥ 1 */

    /* Cache of primes up to the largest p seen so far */
    static idx_t     max_limit = 0;
    static size_t    prime_cnt = 0;
    static idx_t   *prime_list = NULL;

    if (p > max_limit) {              /* Need a larger sieve range */
        free(prime_list);              /* free any previous allocation */
        build_primes(p, &prime_list, &prime_cnt);
        max_limit = p;
    }

    seq_t sum = 0;                     /* sum of distinct primes in C(p, n) */
    idx_t sub = p - n;                 /* p – n */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t q = prime_list[i];
        if (q > p) break;

        /* Legendre’s formula for the exponent of q in C(p, n) */
        idx_t e = 0;
        idx_t power = q;
        while (power <= p) {
            e += p / power - n / power - sub / power;
            if (power > p / q) break;     /* avoid overflow of power *= q   */
            power *= q;
        }

        if (e > 0) {
            sum += (seq_t)q;              /* q divides the binomial coeff. */
        }
    }

    return sum;
}
