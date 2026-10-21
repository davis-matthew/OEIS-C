/*
 * A298404  a(n) = 2*a(n-1) - a(n-3) + a(ceiling(n/2)), where a(0) = 1, a(1) =
 *          1, a(2) = 1.
 * https://oeis.org/A298404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(ceil(n/2)), a(0)=1,a(1)=1,a(2)=1
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t computed = 2;

    /* minimum size to hold 0..2 */
    if (cache_len < 3) {
        cache = (seq_t*)realloc(cache, 3 * sizeof(seq_t));
        cache_len = 3;
    }
    cache[0] = cache[1] = cache[2] = (seq_t)1;

    /* ensure room for the requested index */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache_len = needed;
    }

    if (n < 3) return (seq_t)1;

    if (computed < n) {
        idx_t start = computed + 1;
        if (start < 3) start = 3;
        for (idx_t i = start; i <= n; ++i) {
            seq_t v = (seq_t)2 * cache[i - 1];
            v -= cache[i - 3];
            v += cache[(i + 1) / 2];
            cache[i] = v;
        }
        computed = n;
    }
    return cache[n];
}
#else
#include <vector>
seq_t a(idx_t n)
{
    static std::vector<seq_t> cache;
    if (cache.size() < 3) cache.resize(3, (seq_t)1);
    else { cache[0] = cache[1] = cache[2] = (seq_t)1; }

    if (n < 3) return (seq_t)1;

    if (cache.size() <= n) cache.resize(n + 1);
    static idx_t computed = 2;

    if (computed < n) {
        idx_t start = computed + 1;
        if (start < 3) start = 3;
        for (idx_t i = start; i <= n; ++i) {
            seq_t v = (seq_t)2 * cache[i - 1];
            v -= cache[i - 3];
            v += cache[(i + 1) / 2];
            cache[i] = v;
        }
        computed = n;
    }
    return cache[n];
}
#endif
