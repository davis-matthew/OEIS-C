/*
 * A386332  Primes without {1, 6} as digits.
 * https://oeis.org/A386332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime that contains no decimal digit 1 or 6
 * Range:     u8 n <= 21, u16 n <= 1899, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed by searching for primes and caching the results. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static uint64_t cand = 2;   /* next candidate to test */

    if (n == 0)
        return 0;               /* offset is 1, ignore n=0 */

    /* Expand cache until it contains the nth element */
    while (cache_len < n)
    {
        /* Search until a qualifying prime is found */
        for (;;)
        {
            if (cand < 2)
                cand = 2;

            if (is_prime(cand))
            {
                uint64_t tmp = cand;
                bool bad = false;
                while (tmp != 0)
                {
                    uint8_t d = tmp % 10;
                    if (d == 1 || d == 6)
                    {
                        bad = true;
                        break;
                    }
                    tmp /= 10;
                }
                if (!bad)
                    break; /* found a valid prime */
            }
            cand++;               /* try next candidate */
        }

        /* Store the found prime in the cache */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? 2 * cache_cap : 128;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache)
                exit(EXIT_FAILURE);  /* allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len] = (seq_t)cand;
        cache_len++;
        cand++;                       /* next search starts after this prime */
    }

    return cache[n - 1];
}
