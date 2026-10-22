/*
 * A365783  a(n) = squarefree kernel of A126706(n).
 * https://oeis.org/A365783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree kernel of A126706(n) * A126706(n) is the n-th
 *            composite integer that is NOT a prime power * and contains at
 *            least one squared prime factor. a(n) is its * squarefree kernel
 *            (product of its distinct prime divisors)
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;         /* array of terms */
    static size_t cache_cap = 0;        /* capacity of cache */
    static size_t cache_len = 0;        /* number of terms stored */

    /* Next integer to test (start after 3) */
    static idx_t next_m = 4;

    /* Grow cache until we have at least n terms */
    while (cache_len < n)
    {
        for (;;)
        {
            /* Factorize next_m */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64((uint64_t)next_m, primes, exps, &cnt);

            /* Check for at least two distinct primes, and one repeated prime */
            bool good = false;
            if (cnt >= 2)
            {
                for (size_t i = 0; i < cnt; ++i)
                {
                    if (exps[i] >= 2)
                    {
                        good = true;
                        break;
                    }
                }
            }

            if (good)
            {
                /* Compute the squarefree kernel (product of distinct primes) */
                seq_t rad = (seq_t)1;
                for (size_t i = 0; i < cnt; ++i)
                    rad *= (seq_t)primes[i];

                /* Ensure there is space in the cache */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = rad;

                /* Move to the next candidate for the next loop */
                ++next_m;
                break;          /* we have found this term */
            }
            /* Candidate did not satisfy condition; try next integer */
            ++next_m;
        }
    }

    /* Safely index into cache (idx_t is unsigned) */
    return cache[(size_t)n - 1];
}
