/*
 * A299261  Partial sums of A299255.
 * https://oeis.org/A299261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n > 5, with initial values: a(0)=1, a(1)=8, a(2)=31,
 *            a(3)=81, a(4)=168, a(5)=303
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static const seq_t init[6] = {1, 8, 31, 81, 168, 303};

seq_t a(idx_t n)
{
    /* For n < 6 we have the initial values directly. */
    if (n < 6)
        return init[n];

    /* Initialize cache with the first six values. */
    if (cache_size == 0)
    {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* allocation failed */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if necessary and compute missing terms. */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* allocation failed */
        cache = tmp;

        /* Compute terms from old cache_size up to n. */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            seq_t pos = 3 * cache[i - 1] + 2 * cache[i - 3] + 3 * cache[i - 5];
            seq_t neg = 3 * cache[i - 2] + 3 * cache[i - 4] + cache[i - 6];
            cache[i] = pos - neg;  /* guaranteed to be non‑negative */
        }
        cache_size = new_size;
    }

    return cache[n];
}
