/*
 * A271060  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 261", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) for n > 5 * Initial values:
 *            a(0)=1, a(1)=8, a(2)=1, a(3)=48, a(4)=1, a(5)=120
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Pre‑computed first six terms */
static const seq_t init[6] = { (seq_t)1, (seq_t)8, (seq_t)1, (seq_t)48,
                               (seq_t)1, (seq_t)120 };

#ifdef __cplusplus
/* C++ implementation: dynamic array via std::vector */
static std::vector<seq_t> cache;
#else
/* C implementation: raw pointer */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_max = 0;   /* highest index computed and stored */
#endif

seq_t a(idx_t n)
{
    /* Small indices are handled directly */
    if (n <= 5) {
        return init[(size_t)n];
    }

#ifdef __cplusplus
    /* Ensure the vector contains the initial six terms */
    if (cache.empty()) {
        cache.reserve(64u);
        for (size_t i = 0; i < 6u; ++i) {
            cache.push_back(init[i]);
        }
    }

    /* Compute terms up to n (forward only) */
    while (cache.size() <= n) {
        size_t i = cache.size();
        seq_t val = (seq_t)3 * cache[i-2] - (seq_t)3 * cache[i-4] + cache[i-6];
        cache.push_back(val);
    }
    return cache[(size_t)n];
#else
    /* C implementation: allocate / grow a raw array */
    size_t sn = (size_t)n;

    /* First allocation */
    if (cache == NULL) {
        size_t cap = sn + 1;
        if (cap < 64u) cap = 64u;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 6u; ++i) {
            cache[i] = init[i];
        }
        /* Zero remaining part (optional) */
        for (size_t i = 6u; i < cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_cap = cap;
        cache_max = 5u;
    }
    /* Ensure sufficient capacity */
    if (sn >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2u : 64u;
        while (newcap <= sn) {
            newcap <<= 1u;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        /* Zero the newly allocated space */
        for (size_t i = cache_cap; i < newcap; ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cache_cap = newcap;
    }
    /* Compute missing terms */
    if (sn > cache_max) {
        for (size_t i = cache_max + 1u; i <= sn; ++i) {
            cache[i] = (seq_t)3 * cache[i-2] - (seq_t)3 * cache[i-4] + cache[i-6];
        }
        cache_max = sn;
    }
    return cache[sn];
#endif
}
