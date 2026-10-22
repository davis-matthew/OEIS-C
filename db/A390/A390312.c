/*
 * A390312  Numbers k such that sigma(k) has at least one prime factor larger
 *          than the largest prime factor of k.
 * https://oeis.org/A390312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2447, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390312:
   Numbers k such that sigma(k) has at least one prime factor larger
   than the largest prime factor of k.
   The function generates the sequence incrementally, caching results
   to allow random access in any order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"   /* largest_prime_factor(uint64_t) */
#include "../utils/sigma_of.h"              /* sigma_of(idx_t)       */

seq_t a(idx_t n) {
    /* Static cache of sequence elements */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    /* Last tested integer */
    static idx_t next = 1;   /* will start testing from 2 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        ++next;                           /* candidate k > 1 */
        uint64_t k = (uint64_t)next;      /* fits in 64 bits for our search range */

        /* largest prime factor of k */
        uint64_t lpf_k = largest_prime_factor(k);

        /* sum of divisors of k */
        seq_t sigma_k = sigma_of(next);

        /* convert sigma to 64‑bit for factorisation */
        uint64_t sigma_u = (uint64_t)sigma_k;

        /* largest prime factor of sigma(k) */
        uint64_t lpf_sigma = largest_prime_factor(sigma_u);

        /* If sigma has a prime factor larger than k's largest prime factor,
           accept k as next element of the sequence. */
        if (lpf_sigma > lpf_k) {
            if (len == cap) {
                cap <<= 1;                /* double capacity */
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)next;
        }
    }

    /* Return the requested term (offset 1 → a(1) = cache[0]) */
    return cache[n - 1];
}
