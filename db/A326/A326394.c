/*
 * A326394  Expansion of Sum_{k>=1} x^k * (1 + x^(2*k)) / (1 - x^(3*k)).
 * https://oeis.org/A326394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are NOT congruent to 2 mod 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, but harmless */

    /* Recompute the cache if the requested index is beyond current capacity. */
    if (n > cache_cap)
    {
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;  /* allocation failed */

        /* clear previous cache */
        if (cache) free(cache);
        cache = new_cache;
        cache_cap = n;

        /* initialise counts to zero */
        for (idx_t i = 1; i <= cache_cap; ++i) cache[i] = 0;

        /* sieve: for each divisor d that is NOT congruent to 2 modulo 3,
           increment the count of all multiples of d. */
        for (idx_t d = 1; d <= cache_cap; ++d)
        {
            if ((d % 3) != 2)          /* keep only divisors whose remainder is 0 or 1 */
            {
                for (idx_t m = d; m <= cache_cap; m += d)
                    cache[m] += 1;
            }
        }
    }

    return cache[n];
}
