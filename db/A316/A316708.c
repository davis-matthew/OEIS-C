/*
 * A316708  Bisection of the odd-indexed Pell numbers A001653: part 1.
 * https://oeis.org/A316708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 34*a(n-1) - a(n-2), a(0)=1, a(1)=29
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n)
{
    /* Handle n < 2 directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;

#ifdef __cplusplus
    static std::vector<seq_t> cache;

    /* Initialize the first two terms lazily */
    if (cache.empty())
    {
        cache.resize(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
    }

    /* Grow the cache if necessary */
    if (cache.size() <= (size_t)n)
    {
        cache.resize((size_t)n + 1);
        for (size_t i = 2; i <= (size_t)n; ++i)
        {
            cache[i] = (seq_t)34 * cache[i - 1] - cache[i - 2];
        }
    }

    return cache[(size_t)n];
#else
    /* C implementation using a dynamic array */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of elements initialized */

    /* Lazy initialization */
    if (cache_len == 0)
    {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache_len = 2;
    }

    /* Grow the array if needed */
    if (cache_len <= (size_t)n)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        cache[i] = (seq_t)34 * cache[i - 1] - cache[i - 2];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
#endif
}
