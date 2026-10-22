/*
 * A321504  Numbers k such that k and k+1 each have at least 4 distinct prime
 *          factors.
 * https://oeis.org/A321504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the n‑th integer k such that k and k+1 each have at least
   four distinct prime factors.  The function keeps a static cache of
   found terms so that calls with arbitrary n order remain fast.  */

#include <stdlib.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Static cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored        */
    static size_t cache_cap  = 0;   /* allocated capacity               */
    static uint64_t next_k   = 1;   /* next integer to test (k >= 1)    */

    /* Ensure we have at least n terms in the cache. */
    while (cache_size < n)
    {
        /* Find the next qualifying k */
        while (1)
        {
            uint64_t k = next_k++;
            if (count_distinct_primes(k)     >= 4 &&
                count_distinct_primes(k + 1) >= 4)
            {
                /* Grow cache if necessary */
                if (cache_size == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();  /* out of memory – terminate */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                /* Store found term */
                cache[cache_size++] = (seq_t)k;
                break;               /* proceed to next cache element */
            }
        }
    }
    return cache[n - 1];
}
