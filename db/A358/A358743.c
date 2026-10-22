/*
 * A358743  First of three consecutive primes p,q,r such that p+q-r is prime.
 * https://oeis.org/A358743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) searches the sequence of consecutive primes p,q,r such that
 *            p+q‑r is prime
 * Range:     u8 n <= 23, u16 n <= 1295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed values so repeated or out‑of‑order calls are efficient.
   It generates primes on the fly using next_prime() and tests primality of the small
   number p+q‑r using is_prime().  The computed value is stored in a static dynamic array. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/next_prime.h"   // declares uint64_t next_prime(uint64_t start);
#include "../utils/is_prime.h"     // declares bool is_prime(idx_t x);

seq_t a(idx_t n)
{
    /* static cache of results */
    static seq_t *cache = NULL;        /* array of found terms */
    static size_t cap   = 0;           /* capacity of cache */
    static size_t sz    = 0;           /* number of computed terms */

    /* state for generating the next qualifying prime */
    static uint64_t p = 0, q = 0, r = 0;
    static bool initialized = false;

    /* initialise on first call */
    if (!initialized) {
        /* start with the first three consecutive primes 2,3,5 */
        p = 2;
        q = next_prime(p);
        r = next_prime(q);

        /* allocate initial cache */
        cap = 16; /* arbitrary small start, will grow as needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) {
            /* out of memory; return zero on failure – caller should handle */
            return (seq_t)0;
        }
        initialized = true;
    }

    /* compute until we have at least n terms */
    while (sz < n) {
        /* compute p + q - r safely */
        int64_t val = (int64_t)p + (int64_t)q - (int64_t)r;

        if (val >= 2 && is_prime((idx_t)val)) {
            /* store the qualifying prime */
            if (sz == cap) {
                /* grow cache */
                size_t new_cap = cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* out of memory; return zero on failure – caller should handle */
                    return (seq_t)0;
                }
                cache = new_cache;
                cap = new_cap;
            }
            cache[sz] = (seq_t)p;
            sz++;
        }

        /* advance to the next triple of consecutive primes */
        p = q;
        q = r;
        r = next_prime(r);
    }

    return cache[n - 1];
}
