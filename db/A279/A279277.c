/*
 * A279277  Composition of Lucas numbers A000032 with Fibonacci numbers
 *          A000045.
 * https://oeis.org/A279277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+a(n-2)-3*a(n-3)-a(n-4); a(1)=1, a(2)=4, a(3)=12,
 *            a(4)=37
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoized computation of A279277 (composition of Lucas and Fibonacci numbers). */
seq_t a(idx_t n)
{
    /* Basic values for n <= 4. */
    static const seq_t init[4] = { (seq_t)1, (seq_t)4, (seq_t)12, (seq_t)37 };

    if (n <= 4)
        return init[(size_t)n - 1U];

    /* Static cache: index 1..cache_len are valid. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache can hold index n. */
    if (cache_len < (size_t)n)
    {
        size_t new_len = (size_t)n;
        if (cache_len == 0)
        {
            /* Allocate space and initialise first four entries. */
            cache = (seq_t *)malloc((new_len + 1) * sizeof(seq_t));
            if (!cache)
                return (seq_t)0; /* allocation failure; fallback value */
            for (size_t i = 1; i <= 4; ++i)
                cache[i] = init[i - 1U];
            cache_len = 4;
        }
        else
        {
            cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
            if (!cache)
                return (seq_t)0;
            /* All entries up to cache_len already filled. */
        }

        /* Compute terms from cache_len+1 up to n. */
        for (size_t i = cache_len + 1; i <= new_len; ++i)
        {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            cache[i] = (3 * a1) + a2 - (3 * a3) - a4;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
