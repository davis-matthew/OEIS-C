/*
 * A268526  a(n) = r*a(ceiling(n/2))+s*a(floor(n/2)) with a(1)=1 and
 *          (r,s)=(3,2).
 * https://oeis.org/A268526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(ceil(n/2))+2*a(floor(n/2)), a(1)=1
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#ifdef __cplusplus
static std::vector<seq_t> cacheVec;
static idx_t last_computed_vec = 0;
#else
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t last_computed = 0;
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;                          // safety (offset is 1)
#ifdef __cplusplus
    if (n >= cacheVec.size()) cacheVec.resize(n + 1);
    if (last_computed_vec < 1) {
        cacheVec[1] = 1;
        last_computed_vec = 1;
    }
    for (idx_t i = last_computed_vec + 1; i <= n; ++i) {
        idx_t half = i / 2;
        if ((i & 1) == 0) {                       // even n
            cacheVec[i] = (seq_t)5 * cacheVec[half];
        } else {                                  // odd n
            cacheVec[i] = (seq_t)3 * cacheVec[half + 1]
                         + (seq_t)2 * cacheVec[half];
        }
    }
    last_computed_vec = n;
    return cacheVec[n];
#else
    if (n >= cache_size) {
        seq_t *new_cache = realloc(cache, sizeof(seq_t) * (n + 1));
        cache = new_cache;
        cache_size = n + 1;
    }
    if (last_computed < 1) {
        cache[1] = 1;
        last_computed = 1;
    }
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t half = i / 2;
        if ((i & 1) == 0) {                       // even n
            cache[i] = (seq_t)5 * cache[half];
        } else {                                  // odd n
            cache[i] = (seq_t)3 * cache[half + 1]
                         + (seq_t)2 * cache[half];
        }
    }
    last_computed = n;
    return cache[n];
#endif
}
