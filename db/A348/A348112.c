/*
 * A348112  a(n) = t(n)*a(n-1) + a(n-2) for n>1 where t(n) is the
 *          Prouhet-Thue-Morse sequence A106400 with a(0)=0 and a(1)=1.
 * https://oeis.org/A348112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=1, a(n)=t(n)*a(n-1)+a(n-2) for n>1, where
 *            t(n)=(-1)^{popcount(n)} (1 if popcount(n) even, -1 otherwise)
 * Range:     i8 n <= 33, i16 n <= 73, i32 n <= 153, i64 n <= 309,
 *            i128 n <= 621, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

/* Portable popcount for a 64‑bit unsigned integer. */
static inline unsigned int popcount_u64(idx_t x) {
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* t(n) = (-1)^{popcount(n)} as seq_t. */
static inline seq_t t_val(idx_t n) {
    return (popcount_u64(n) & 1) ? (seq_t)-1 : (seq_t)1;
}

#ifdef __cplusplus
/* C++: dynamic vector cache. */
static std::vector<seq_t> cache;

/* Ensure that a[0..n] are computed. */
static void ensure_cached(idx_t n) {
    if (n < 2) {                       /* n==0 or 1 */
        if (cache.empty()) {
            cache.resize(2);
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
        }
        return;
    }
    if (cache.size() <= n) {           /* need to grow */
        size_t oldsize = cache.size();
        cache.resize(n + 1);            /* new elements default‑constructed */
        if (oldsize == 0) {             /* first time ever */
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            oldsize = 2;
        }
        for (idx_t i = oldsize; i <= n; ++i) {
            seq_t t = t_val(i);
            cache[i] = t * cache[i - 1] + cache[i - 2];
        }
    }
}
#else
/* C: pointer array cache. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_size = 0;  /* number of values stored (indices 0..cache_size-1) */

/* Ensure that a[0..n] are computed. */
static void ensure_cached(idx_t n) {
    if (cache != NULL && n < cache_size) return;   /* already cached */

    /* Allocate or grow the array. */
    if (cache == NULL) {
        cache_cap = (n < 4) ? 4 : n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t t = t_val(i);
            cache[i] = t * cache[i - 1] + cache[i - 2];
        }
    }
    cache_size = n + 1;
}
#endif

/* Public function required by the build system. */
seq_t a(idx_t n) {
    ensure_cached(n);
#ifdef __cplusplus
    return cache[n];
#else
    return cache[n];
#endif
}
