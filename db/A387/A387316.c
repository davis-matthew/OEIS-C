/*
 * A387316  Expansion of 1/((1-x) * (1-13*x))^(7/2).
 * https://oeis.org/A387316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n*a(n) = (14*n+35)*a(n-1) - 13*(n+5)*a(n-2) for n > 1 a(0) = 1,
 *            a(1) = 49
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)49;

#ifdef __cplusplus
    /* C++ implementation with a std::vector cache */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(2);
        cache.push_back((seq_t)1);   /* a(0) */
        cache.push_back((seq_t)49);  /* a(1) */
    }
    while (cache.size() <= n) {
        idx_t i = (idx_t)cache.size();          /* next index to compute */
        seq_t term1 = (seq_t)(14*i + 35) * cache[i-1];
        seq_t term2 = (seq_t)(13*(i + 5)) * cache[i-2];
        seq_t num   = term1 - term2;
        cache.push_back(num / i);
    }
    return cache[(size_t)n];
#else
    /* C implementation with a dynamic array cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();            /* allocation failure */
        cache[0] = (seq_t)1;                   /* a(0) */
        cache[1] = (seq_t)49;                  /* a(1) */
    }
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) abort();        /* allocation failure */
        cache = new_cache;
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            unsigned __int128 t1 = (unsigned __int128)(14*i + 35) * cache[i-1];
            unsigned __int128 t2 = (unsigned __int128)(13*(i + 5)) * cache[i-2];
            unsigned __int128 num = t1 - t2;
            cache[i] = (seq_t)(num / (unsigned __int128)i);
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
#endif
}
