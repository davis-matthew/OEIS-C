/*
 * A276693  a(n) = a(n-2)*a(n-3) - a(n-1); a(0) = 3, a(1) = 5, a(2) = 7.
 * https://oeis.org/A276693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)*a(n-3) - a(n-1); a(0) = 3, a(1) = 5, a(2) = 7
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    /*----- C++ build: use std::vector for automatic construction/destruction -----*/
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.reserve(4);
        cache.push_back((seq_t)3);
        cache.push_back((seq_t)5);
        cache.push_back((seq_t)7);
    }

    if (n < cache.size())
        return cache[(size_t)n];

    const size_t old_size = cache.size();
    cache.resize((size_t)n + 1);
    for (idx_t i = (idx_t)old_size; i <= n; ++i)
    {
        cache[(size_t)i] = cache[(size_t)i - 2] * cache[(size_t)i - 3] - cache[(size_t)i - 1];
    }
    return cache[(size_t)n];
#else
    /*----- C build: use raw memory allocation -----*/
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Ensure enough capacity for index n. */
    if (n >= cache_cap)
    {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Initialize base terms on first use. */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)3;
        if (cache_cap > 1)
            cache[1] = (seq_t)5;
        if (cache_cap > 2)
            cache[2] = (seq_t)7;
        cache_len = cache_cap > 2 ? 3 : cache_cap;
    }

    if (n < cache_len)
        return cache[(size_t)n];

    for (idx_t i = cache_len; i <= n; ++i)
    {
        cache[(size_t)i] = cache[(size_t)i - 2] * cache[(size_t)i - 3] - cache[(size_t)i - 1];
    }
    cache_len = n + 1;
    return cache[(size_t)n];
#endif
}
