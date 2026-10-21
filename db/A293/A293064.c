/*
 * A293064  Number of vertices of type B at level n of the hyperbolic Pascal
 *          pyramid PP_(4,5).
 * https://oeis.org/A293064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3), a(0)=0, a(1)=0, a(2)=0,
 *            a(3)=1
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static idx_t max_computed = 0;
#ifdef __cplusplus
static std::vector<seq_t> cache;
static void ensure_cache(idx_t n)
{
    if (cache.size() <= (size_t)n)
        cache.resize(n + 1);
}
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)1;
    ensure_cache(n);
    if (max_computed <= 3) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        max_computed = 3;
    }
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] - (seq_t)4 * cache[i-2] + cache[i-3];
    }
    max_computed = n;
    return cache[n];
}
