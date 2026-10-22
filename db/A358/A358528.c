/*
 * A358528  a(n) = n-th prime p(k) such that p(k) - p(k-1) > p(k-1) - p(k-2).
 * https://oeis.org/A358528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   append_candidate, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A358528 – primes where each successive gap is larger than
 * the preceding one (offset 1). This implementation builds the sequence
 * incrementally, caching computed primes and their gaps. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static data about the last processed prime and its gap */
    static uint64_t last_prime = 5;      /* p_k   = 5 for the first element */
    static uint64_t last_gap   = 2;      /* p_k - p_{k-1} = 5 - 3 */

    /* initialise cache on first call */
    if (cache_len == 0)
    {
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)5);
        /* last_prime and last_gap already set to 5 and 2 */
    }

    /* extend the cache until we have at least n elements */
    if (n > cache_len)
    {
        uint64_t cand = last_prime + 2; /* start scanning from the next odd number */

        while (cache_len < n)
        {
            if (is_prime(cand))
            {
                uint64_t new_gap = cand - last_prime; /* gap between cand and last_prime */
                if (new_gap > last_gap)
                {
                    append_candidate(&cache, &cache_len, &cache_cap, (seq_t)cand);
                }
                /* regardless of whether we added it, update the gap and last_prime */
                last_gap = new_gap;
                last_prime = cand;
            }
            cand += 2; /* advance to next odd candidate */
        }
    }

    return cache[n - 1];
}
