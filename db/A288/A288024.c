/*
 * A288024  Prime p1 of consecutive primes p1, p2, where p2 - p1 = 8, and p1,
 *          p2 are in different decades.
 * https://oeis.org/A288024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 423, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A288024:
   a(n) = the prime p1 such that p2 = p1 + 8 is prime, the two primes are consecutive,
   and floor(p1/10) ≠ floor(p2/10).  The sequence starts at n = 1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* cached values */
static seq_t      *cache          = NULL;
static idx_t       cache_capacity = 0;
static idx_t       cache_size     = 0;
static idx_t       next_candidate = 3;  /* first odd to consider */

/* Ensure that at least 'need' terms are cached. */
static void ensure_cache(idx_t need)
{
    if (need <= cache_size)
        return;

    /* initialise cache array if necessary */
    if (cache == NULL) {
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    }

    /* find further terms until we have 'need' of them */
    while (cache_size < need) {
        /* scan odd candidates until a term is found */
        while (true) {
            idx_t p = next_candidate;

            /* only numbers that can be prime have last digit 1,3,7,9 */
            if (p % 10 == 1 || p % 10 == 3 || p % 10 == 7 || p % 10 == 9) {
                if (is_prime(p)) {
                    idx_t q = p + 8;
                    /* q may overflow the 64‑bit type; in that unlikely case stop */
                    if (q <= UINT64_MAX &&
                        is_prime(q) &&
                        /* ensure p and q are consecutive primes */
                        next_prime(p) == q &&
                        /* ensure different decades */
                        (p / 10) != (q / 10)) {

                        /* grow cache if needed */
                        if (cache_size == cache_capacity) {
                            cache_capacity <<= 1;
                            cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
                        }
                        cache[cache_size++] = (seq_t)p;

                        /* prepare to continue scanning after this prime */
                        next_candidate = p + 2;
                        break;   /* found next term */
                    }
                }
            }
            next_candidate += 2;
            if (next_candidate > UINT64_MAX - 8)
                return; /* safety guard – sequence terms will fit before this */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined for n<1 – return 0 safely */
    ensure_cache(n);
    return cache[n - 1];
}
