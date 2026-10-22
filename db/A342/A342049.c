/*
 * A342049  Primes formed by the concatenation of exactly two consecutive
 *          composite numbers.
 * https://oeis.org/A342049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   concat_parts, ensure_capacity, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th prime that is the concatenation of two
   consecutive composite numbers.  The function uses incremental
   search with memoisation so that calls with arbitrary indices
   are answered in O(n) time. */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/concat_parts.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Static memoisation data */
    static seq_t *cache = NULL;          /* stored primes */
    static size_t cache_cap = 0;         /* allocated capacity */
    static idx_t cache_len = 0;          /* number of stored terms */
    static idx_t last_m = 3;             /* last candidate integer (starts below 4) */

    /* Grow the cache until we have the requested term */
    while (cache_len < n)
    {
        /* Search for the next qualifying pair (m, m+1) */
        bool found = false;
        while (!found)
        {
            /* Advance to the next integer */
            ++last_m;

            /* Skip until m is composite and m+1 is composite */
            if (last_m <= 3) continue;                 /* 0,1,2,3 are not composite */
            if (is_prime_small((seq_t)last_m)) continue;
            if (is_prime_small((seq_t)(last_m + 1))) continue;

            /* Concatenate the two numbers and test for primality */
            seq_t val = concat_parts(last_m, last_m + 1);
            if (is_prime_big(val))
            {
                /* Append to the cache */
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = val;
                found = true;
            }
        }
    }

    return cache[n - 1];
}
