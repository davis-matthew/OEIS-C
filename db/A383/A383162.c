/*
 * A383162  Consecutive states of the linear congruential pseudo-random number
 *          generator (7200*s + 1) mod 23^4 when started at s=1.
 * https://oeis.org/A383162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7200*a(n-1) + 1) mod 23^4, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;
static idx_t cache_len   = 0;

static const seq_t MODULO     = (seq_t)279841; /* 23^4 */
static const seq_t MULTIPLIER = (seq_t)7200;
static const seq_t ONE        = (seq_t)1;

seq_t a(idx_t n)
{
    /* a(1) is the first valid term; behaviour for n==0 is undefined. */
    if (n == 0)
        return (seq_t)0;

    if (n > cache_len)
    {
        /* Allocate or grow the cache. */
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;            /* allocation failure */

        cache = new_cache;

        /* Ensure the first term is present before generating successors. */
        if (cache_len == 0)
        {
            cache[0] = ONE;              /* a(1) = 1 */
            cache_len = 1;
        }

        /* Compute the missing terms up to n. */
        for (idx_t i = cache_len; i < n; ++i)
        {
            cache[i] = (MULTIPLIER * cache[i - 1] + ONE) % MODULO;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
