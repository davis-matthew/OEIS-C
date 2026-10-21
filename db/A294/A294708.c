/*
 * A294708  Number of permutations of [n] avoiding {4231, 2143, 1234}.
 * https://oeis.org/A294708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 258, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)   (n > 10)
   a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 21, a(5) = 69,
   a(6) = 190, a(7) = 446, a(8) = 927, a(9) = 1745, a(10) = 3036
*/

#include <stdlib.h>

static const seq_t init_vals[11] = {1,1,2,6,21,69,190,446,927,1745,3036};

static seq_t *cache = NULL;          /* cache[0 .. cache_size-1] */
static idx_t cache_size = 0;         /* number of cached entries */

/* Extend the cache so that it contains all values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_size >= needed)
        return;                      /* already enough */

    if (cache == NULL)
    {
        cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (!cache)
            return; /* allocation failed; caller will receive garbage */

        /* Initialize the first 11 terms */
        size_t upto = needed < 11 ? needed : 11;
        for (size_t i = 0; i < upto; ++i)
            cache[i] = init_vals[i];

        /* Compute further terms using the recurrence */
        for (size_t i = 11; i < needed; ++i)
        {
            cache[i] = 5 * cache[i - 1]
                       - 10 * cache[i - 2]
                       + 10 * cache[i - 3]
                       - 5  * cache[i - 4]
                       +  cache[i - 5];
        }
        cache_size = needed;
    }
    else
    {
        /* Grow the existing cache */
        size_t old_size = (size_t)cache_size;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (!cache)
            return; /* allocation failed */

        for (size_t i = old_size; i < needed; ++i)
        {
            if (i < 11)
                cache[i] = init_vals[i];
            else
                cache[i] = 5 * cache[i - 1]
                           - 10 * cache[i - 2]
                           + 10 * cache[i - 3]
                           - 5  * cache[i - 4]
                           +  cache[i - 5];
        }
        cache_size = needed;
    }
}

/* Return the n‑th term of A294708. */
seq_t a(idx_t n)
{
    if (n <= 10)
        return init_vals[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
