/*
 * A267807  a(0) = a(1) = 1; for n>1, a(n) = (a(n-1) mod 3) + a(n-2).
 * https://oeis.org/A267807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; for n>1, a(n) = (a(n-1) mod 3) + a(n-2)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* static state */
static idx_t cache_max = 0;
#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* First call: initialise cache */
    if (cache.empty())
    {
        cache.resize(2);
        cache[0] = 1;
        cache[1] = 1;
        cache_max = 1;
    }
    /* Ensure capacity */
    if (n >= (idx_t)cache.size())
        cache.resize(n + 1);
#else
    if (cache == NULL)
    {
        size_t init_cap = (size_t)(n + 1 > 4 ? n + 1 : 4);
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* out of memory */
        cache[0] = 1;
        if (init_cap > 1) cache[1] = 1;
        cache_cap = init_cap;
        cache_max = (init_cap > 1) ? 1 : 0;
    }
    if ((size_t)n + 1 > cache_cap)
    {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }
#endif

    /* Compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
#ifdef __cplusplus
        cache[i] = (cache[i-1] % (seq_t)3) + cache[i-2];
#else
        cache[i] = (cache[i-1] % (seq_t)3) + cache[i-2];
#endif
    }
    cache_max = (cache_max < n) ? n : cache_max;

    return
#ifdef __cplusplus
        cache[n];
#else
        cache[n];
#endif
}
