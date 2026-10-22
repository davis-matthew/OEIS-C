/*
 * A364849  Number of even parity alternating permutations of [1..n] starting
 *          with an odd integer.
 * https://oeis.org/A364849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010551(n)/2 for n>2 Sequence implements recurrence: a(1)
 *            = a(2) = a(3) = a(4) = 1; for n>=5: a(n) = a(n-1) * floor(n/2)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 17, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;      /* table of computed terms */
static idx_t  cache_size = 0;   /* allocated size (indices 0..cache_size-1) */
static idx_t  last_computed = 0;

/* enlarge the cache to hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;

    idx_t new_size = n + 1;                 /* need space for 0..n */
    seq_t *new_cache = alloc_cache(new_size); /* allocate new array */

    /* copy existing values */
    if (cache) {
        for (idx_t i = 0; i <= last_computed; ++i)
            new_cache[i] = cache[i];
        free_cache(cache);                  /* release old array */
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n <= 4) return (seq_t)1;           /* initial terms */

    if (!cache) {                          /* first call */
        cache_size = 5;                    /* indices 0..4 */
        cache = alloc_cache(cache_size);
        for (idx_t i = 1; i <= 4; ++i)
            cache[i] = (seq_t)1;
        last_computed = 4;
    }

    if (n > last_computed) {
        /* grow cache if necessary */
        ensure_cache(n);

        /* compute missing terms using the recurrence */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)(i/2);  /* floor(i/2) is integer division */
        }
        last_computed = n;
    }

    return cache[n];
}
