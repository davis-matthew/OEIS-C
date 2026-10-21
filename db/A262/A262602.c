/*
 * A262602  a(n) = a(n-7) + a(n-4) + a(n-1) for n>1 and a(n)=1 for n<=1.
 * https://oeis.org/A262602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) + a(n-4) + a(n-1) for n>1, a(n) = 1 for n <= 1
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed = 0;

seq_t a(idx_t n)
{
    /* Base values for all n <= 1  */
    if (n <= 1)
        return (seq_t)1;

    /* If we already have the desired value cached, return it */
    if (cache && n < computed)
        return cache[n];

    /* Make sure the cache can hold 0 … n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute from the first missing index up to n (inclusive) */
    for (idx_t i = computed; i <= n; ++i)
    {
        if (i <= 1)
        {
            /* This branch is reached only for i == 0 or i == 1
               (or for an unlikely call with computed < 0).  */
            cache[i] = (seq_t)1;
        }
        else
        {
            /* Recurrence with the convention a(k) = 1 for k <= 1 */
            seq_t sum = cache[i - 1];

            if (i >= 4)
                sum += cache[i - 4];
            else
                sum += (seq_t)1;

            if (i >= 7)
                sum += cache[i - 7];
            else
                sum += (seq_t)1;

            cache[i] = sum;
        }
    }

    /* Now we have computed up to index n (inclusive) */
    computed = n + 1;
    return cache[n];
}
