/*
 * A398040  G.f. A(x) satisfies A(x) = A(x^2) / (1-4*x), with A(0) = 1.
 * https://oeis.org/A398040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + (n even ? a(n/2) : 0), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
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
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty())
        cache.reserve(1), cache.push_back((seq_t)1);          /* a(0) */

    if (n >= cache.size()) {
        cache.reserve(n + 1);                                 /* ensure capacity */
        for (idx_t i = cache.size(); i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else {
                seq_t val = (seq_t)4 * cache[i - 1];
                if ((i & 1) == 0)                          /* even */
                    val += cache[i >> 1];
                cache[i] = val;
            }
        }
    }
    return cache[(size_t)n];
#else
    static seq_t *cache = NULL;         /* allocated cache */
    static idx_t cache_size = 0;         /* number of elements stored */

    idx_t required = n + 1;
    if (required > cache_size) {
        /* reallocate enough space */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * required);
        if (!new_cache) return (seq_t)0;  /* out of memory fallback */

        /* compute and store new entries */
        for (idx_t i = cache_size; i < required; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)1;
            else {
                seq_t val = (seq_t)4 * new_cache[i - 1];
                if ((i & 1) == 0)                     /* even */
                    val += new_cache[i >> 1];
                new_cache[i] = val;
            }
        }

        cache = new_cache;
        cache_size = required;
    }
    return cache[n];
#endif
}
