/*
 * A261080  Semiprimes p*q for which p and q are successive primes and their
 *          binary representations differ from each other in one bit position
 *          only.
 * https://oeis.org/A261080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A261080:
   semiprimes p*q where p and q are successive primes and
   popcount(p ^ q) == 1.  The function builds the sequence on demand
   and is not dependent on the order of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* sequence values  */
    static idx_t  cache_len = 0;   /* number of values produced */
    static idx_t  cache_cap = 0;   /* allocated capacity */
    static uint64_t last_prime = 0;/* last prime used in generation */
    static bool    initialized = false;

    if (!initialized) {
        initialized = true;
        last_prime = 2;                    /* start with first prime */
        cache_cap = 8;                     /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t p = last_prime;           /* current prime   */
        uint64_t q = next_prime(p);        /* next successive prime */
        last_prime = q;                    /* update for next iteration */

        /* Check if binary representations differ by exactly one bit */
        if (__builtin_popcountll(p ^ q) == 1) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                if (cache_cap == 0) cache_cap = 8;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            /* Store the semiprime product */
            cache[cache_len++] = (seq_t)p * (seq_t)q;
        }
    }

    return cache[n - 1];
}
