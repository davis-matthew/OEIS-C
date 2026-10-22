/*
 * A367568  a(n) = Product_{k=0..n} (4*k)! / k!^4.
 * https://oeis.org/A367568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} binomial(4*k,k) * binomial(3*k,k) *
 *            binomial(2*k,k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) */
#include <stdlib.h>              /* malloc, realloc, free */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build: use a static std::vector as a cache. */
    static std::vector<seq_t> a_cache;

    if (a_cache.size() <= n) {
        size_t start = a_cache.size();
        if (start == 0) {
            a_cache.push_back(seq_t(1));   /* a(0) = 1 */
            ++start;
        }
        for (size_t k = start; k <= (size_t)n; ++k) {
            idx_t kk = (idx_t)k;
            seq_t term = binomial((idx_t)(4*kk), kk);
            term *= binomial((idx_t)(3*kk), kk);
            term *= binomial((idx_t)(2*kk), kk);
            a_cache.push_back(a_cache.back() * term);
        }
    }
    return a_cache[(size_t)n];
#else
    /* C build: use a dynamically‑resized static array cache. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;          /* OOM guard – returns 0 for safety */
        cache[0] = 1;                  /* a(0) = 1 */
        cache_size = 1;
    }

    /* grow cache if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;        /* OOM guard */
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t k = cache_size; k <= (size_t)n; ++k) {
            idx_t kk = (idx_t)k;
            seq_t term = binomial((idx_t)(4*kk), kk);
            term *= binomial((idx_t)(3*kk), kk);
            term *= binomial((idx_t)(2*kk), kk);
            seq_t prod = cache[k-1];
            prod *= term;
            cache[k] = prod;
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
#endif
}
