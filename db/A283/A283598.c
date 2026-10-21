/*
 * A283598  Numbers k such that all three of 6*k+1, 6*(k+1)+1, and 6*(k+2)+1
 *          are semiprimes.
 * https://oeis.org/A283598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k such that 6*k+1, 6*(k+1)+1, and 6*(k+2)+1
   are all semiprimes.  The routine keeps a growing cache of found k's
   and performs a lightweight O(1) test by counting prime factors
   (product of exactly two primes, counting multiplicity).  */

/* Note: The implementation uses only 64‑bit arithmetic for the test
   since all values in the sequence up to the required range fit in
   uint64_t.  When seq_t is an arbitrary‑precision type, the values
   are cast to that type when stored in the cache.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"

/* Count prime factors of a 64‑bit integer (product of two primes
   including multiplicity).  */
static bool is_semiprime_uint64(uint64_t x)
{
    return count_prime_factors(x) == 2;
}

seq_t a(idx_t n)                 /* returns the nth term, 1‑based */
{
    /* Static cache of found k values */
    static bool initialized   = false;
    static seq_t *cache       = NULL;   /* array of k’s            */
    static size_t cache_len   = 0;       /* current number stored   */
    static size_t cache_cap   = 0;       /* current capacity        */
    static uint64_t last_k    = 0;       /* last k checked           */

    if (!initialized) {
        initialized = true;
    }

    /* Grow cache until we have the nth term */
    while (cache_len < n) {
        uint64_t k   = last_k;            /* current candidate */
        uint64_t v1  = k * 6ULL + 1ULL;   /* 6*k+1             */
        uint64_t v2  = v1 + 6ULL;         /* 6*(k+1)+1         */
        uint64_t v3  = v1 + 12ULL;        /* 6*(k+2)+1         */

        if ( is_semiprime_uint64(v1) &&
             is_semiprime_uint64(v2) &&
             is_semiprime_uint64(v3) )
        {
            /* Append k to cache (grow if necessary) */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)k; /* cast to the build’s seq_t */
            ++cache_len;
        }

        ++last_k;   /* move to next candidate */
    }

    return cache[n - 1];
}
