/*
 * A328686  Define a map from the primes to the primes by f(p) = (p-1)/2 if
 *          that is prime, or else (p+1)/2 if that is prime, and otherwise is
 *          undefined. Start with the n-th prime and iterate f until we cannot
 *          go any further; a(n) is the number of steps.
 * https://oeis.org/A328686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A328686: a(n) = number of steps in the trajectory of the nth prime.
   Each step applies f(p) = (p-1)/2 if it is prime, else (p+1)/2 if that is prime.
   The process stops when neither value is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

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
    if (n == 0) return 0;            /* defensive guard for n<1 */
    idx_t p = (idx_t)get_prime(n);   /* nth prime (1‑based) */
    seq_t steps = 0;

    while (1) {
        idx_t p1 = (p - 1) / 2;
        if (is_prime(p1)) {          /* (p-1)/2 is prime */
            p = p1;
            steps++;
            continue;
        }

        idx_t p2 = (p + 1) / 2;
        if (is_prime(p2)) {          /* (p+1)/2 is prime */
            p = p2;
            steps++;
            continue;
        }

        break;                       /* no further prime transition */
    }
    return steps;
}
