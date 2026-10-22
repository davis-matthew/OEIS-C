/*
 * A371216  a(0) = 1, a(1) = 1; for n >= 2, a(n) = Min(a(n - 1), a(n - 2)) +
 *          GCD(a(n - 1), n - 1).
 * https://oeis.org/A371216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; for n >= 2, a(n) = Min(a(n-1), a(n-2)) +
 *            GCD(a(n-1), n-1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Memoised computation of the sequence.  The cache is grown on demand.
   The implementation is O(n) per call and works for any seq_t and idx_t
   that can be represented in the build. */
seq_t a(idx_t n)
{
    /* static cache data */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (in elements) */
    static idx_t cache_max = 0;    /* highest index stored (initially 0) */

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 2;                    /* store indices 0,1 at first */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initialise the first two terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* if the requested index is already cached, return it immediately */
    if (n <= cache_max)
        return cache[n];

    /* ensure the cache can hold index n (expand if necessary) */
    size_t needed = (size_t)n + 1U;        /* array length needed */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        if (new_cap < needed)
            new_cap = needed;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                /* allocation failure – abort */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1U; i <= n; ++i) {
        seq_t prev1 = cache[i - 1U];
        seq_t prev2 = cache[i - 2U];
        seq_t mn    = (prev1 < prev2) ? prev1 : prev2;
        seq_t g     = gcd(prev1, (seq_t)(i - 1U));
        cache[i]    = mn + g;
    }
    cache_max = n;
    return cache[n];
}
