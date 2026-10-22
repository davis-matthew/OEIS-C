/*
 * A364307  Numbers k such that k, k+1 and k+2 have exactly 2 distinct prime
 *          factors.
 * https://oeis.org/A364307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1715, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A364307:
   numbers k such that k, k+1, and k+2 each have exactly 2 distinct prime factors. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    /* last integer k that we have inspected (k-1 has been checked) */
    static uint64_t last_seen = 0;

    if (n == 0)                 /* impossible for OEIS offset but safe */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        uint64_t k = last_seen + 1;            /* next candidate */

        if (count_distinct_primes(k)      == 2 &&
            count_distinct_primes(k + 1)  == 2 &&
            count_distinct_primes(k + 2)  == 2)
        {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)k);
        }

        last_seen = k;
    }

    return cache[n - 1];
}
