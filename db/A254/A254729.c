/*
 * A254729  Number of numbers j + k*sqrt(2) of length n, where the length is
 *          the least number of steps to reach 0, the allowable steps being x
 *          -> x + 1 and x -> x*sqrt(2).
 * https://oeis.org/A254729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254729: Number of numbers j + k*sqrt(2) of minimal length n.
   Recurrence:   a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=7;
                 a(n)=a(n-1)+a(n-2)  for n ≥ 6        */

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for flexible growth. */
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        /* initialise the first six terms */
        cache.reserve(6);
        cache.push_back((seq_t)1); /* a(0) */
        cache.push_back((seq_t)1); /* a(1) */
        cache.push_back((seq_t)2); /* a(2) */
        cache.push_back((seq_t)3); /* a(3) */
        cache.push_back((seq_t)4); /* a(4) */
        cache.push_back((seq_t)7); /* a(5) */
    }

    /* Grow the sequence up to n if necessary. */
    for (idx_t i = cache.size(); i <= n; ++i)
    {
        if (i < 6)
            cache.push_back((seq_t)0); /* never reached (i < cache.size()) */
        else
            cache.push_back(cache[i - 1] + cache[i - 2]);
    }

    return cache[(size_t)n];
#else
    /* C implementation.  Cache is a grow‑able array of seq_t values. */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;          /* highest index currently stored   */
    const static seq_t base[6] = {
        (seq_t)1, /* a(0) */
        (seq_t)1, /* a(1) */
        (seq_t)2, /* a(2) */
        (seq_t)3, /* a(3) */
        (seq_t)4, /* a(4) */
        (seq_t)7  /* a(5) */
    };

    /* Initialise on first call. */
    if (!cache)
    {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure  */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
        max_cached = 5;
    }

    if (n <= max_cached)
        return cache[(size_t)n];

    /* Grow the array to accommodate n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                 /* allocation failure  */
    cache = tmp;

    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        if (i < 6)
            cache[i] = base[i];
        else
            cache[i] = cache[i - 1] + cache[i - 2];
    }
    max_cached = n;

    return cache[(size_t)n];
#endif
}
