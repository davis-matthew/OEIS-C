/*
 * A245992  Number of length n+2 0..5 arrays with no pair in any consecutive
 *          three terms totalling exactly 5.
 * https://oeis.org/A245992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) for n ≥ 3 Initial values: a(1) = 126,
 *            a(2) = 534
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Computes a(n) for n ≥ 1.  The function is memoised so that calls
   in any order are fast. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* behavior below the offset is irrelevant */

#ifdef __cplusplus
    /* C++ implementation using std::vector for dynamic storage */
    static std::vector<seq_t> cache;

    /* initialise the cache with the first two terms when needed */
    if (cache.size() == 0)
    {
        cache.resize(2);
        cache[0] = (seq_t)126;   /* a(1) */
        cache[1] = (seq_t)534;   /* a(2) */
    }

    /* grow the cache to contain a(1)..a(n) */
    if (cache.size() < (size_t)n)
    {
        size_t old = cache.size();
        cache.resize((size_t)n);
        for (size_t i = old; i < (size_t)n; ++i)
            cache[i] = cache[i - 1] * (seq_t)4 + cache[i - 2];
    }

    return cache[(size_t)n - 1];
#else
    /* C implementation using raw dynamic array */
    static seq_t *cache  = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    /* 'cache_len' is the number of terms currently stored (a(1) .. a(cache_len)) */
    static size_t cache_len = 0;

    /* make room for n elements */
    if (cache_cap < (size_t)n)
    {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp)
            abort();                     /* allocation failure */
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* initialise the first two terms if necessary */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)126;   /* a(1) */
        cache_len = 1;
    }
    if (cache_len == 1)
    {
        cache[1] = (seq_t)534;   /* a(2) */
        cache_len = 2;
    }

    /* compute missing terms up to index n */
    if (cache_len < (size_t)n)
    {
        for (size_t i = cache_len; i < (size_t)n; ++i)
            cache[i] = cache[i - 1] * (seq_t)4 + cache[i - 2];
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
#endif
}
