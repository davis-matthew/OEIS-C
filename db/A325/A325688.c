/*
 * A325688  Number of length-3 compositions of n such that every distinct
 *          consecutive subsequence has a different sum.
 * https://oeis.org/A325688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - a(n-4) - 2*a(n-5) + a(n-7) for n > 7
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=4,
 *            a(6)=5, a(7)=12
 * Range:     u8 n <= 17, u16 n <= 363, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- C++ build -------------------------------------------- */
#ifdef __cplusplus
/* Global cache for C++ builds */
static std::vector<seq_t> cacheVec;

/* Ensure that cacheVec contains values up to index n */
static void ensure_cache(idx_t n)
{
    if (cacheVec.size() <= (size_t)n) {
        size_t oldSize = cacheVec.size();
        cacheVec.resize(n + 1);
        for (size_t k = oldSize; k <= (size_t)n; ++k) {
            if (k < 8) {
                switch ((idx_t)k) {
                case 0: cacheVec[k] = 0; break;
                case 1: cacheVec[k] = 0; break;
                case 2: cacheVec[k] = 0; break;
                case 3: cacheVec[k] = 1; break;
                case 4: cacheVec[k] = 0; break;
                case 5: cacheVec[k] = 4; break;
                case 6: cacheVec[k] = 5; break;
                case 7: cacheVec[k] = 12; break;
                default: break;
                }
            } else {
                seq_t pos = cacheVec[k - 2];
                pos += pos;                      /* 2*a(k-2) */
                pos += cacheVec[k - 3];
                pos += cacheVec[k - 7];
                seq_t neg = cacheVec[k - 4];
                seq_t temp = cacheVec[k - 5];
                temp += temp;                    /* 2*a(k-5) */
                neg += temp;
                cacheVec[k] = (pos >= neg ? pos - neg : 0);
            }
        }
    }
}
#else
/* ---------- C build -------------------------------------------- */

/* Global cache for C builds */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;   /* number of computed elements          */
static size_t cache_cap  = 0;   /* allocated capacity in elements        */

/* Ensure that cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* initialise cache if needed */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        assert(cache != NULL);
    }
    /* grow cache if required */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < (idx_t)(n + 1))
            new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        assert(new_cache != NULL);
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    /* compute new values up to n */
    while (cache_len <= n) {
        idx_t k = cache_len;
        if (k < 8) {
            switch ((int)k) {
            case 0: cache[k] = 0; break;
            case 1: cache[k] = 0; break;
            case 2: cache[k] = 0; break;
            case 3: cache[k] = 1; break;
            case 4: cache[k] = 0; break;
            case 5: cache[k] = 4; break;
            case 6: cache[k] = 5; break;
            case 7: cache[k] = 12; break;
            default: break;
            }
        } else {
            seq_t pos = cache[k - 2];
            pos += pos;                      /* 2*a(k-2) */
            pos += cache[k - 3];
            pos += cache[k - 7];
            seq_t neg = cache[k - 4];
            seq_t temp = cache[k - 5];
            temp += temp;                    /* 2*a(k-5) */
            neg += temp;
            cache[k] = (pos >= neg ? pos - neg : 0);
        }
        ++cache_len;
    }
}
#endif

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
#ifdef __cplusplus
    return cacheVec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
