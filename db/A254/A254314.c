/*
 * A254314  Hankel transform of a(n) is A006720(n). Hankel transform of a(n+1)
 *          is A006720(n+2).
 * https://oeis.org/A254314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence n*a(n) + 2*(-5*n+6)*a(n-1) +
 *            2*(17*n-39)*a(n-2) + 6*(-8*n+27)*a(n-3) + (25*n-114)*a(n-4) +
 *            2*(-n+6)*a(n-5) = 0, with initial values a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=5, a(4)=14
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static seq_t *cache = nullptr;
    static size_t cache_size = 0;
    static size_t computed_len = 0;
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t computed_len = 0;
#endif

    /* Ensure cache large enough */
    if (n + 1 > cache_size) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap == 0) new_cap = 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failed */
        /* copy old data */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        /* zero unused slots */
        for (size_t i = cache_size; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_cap;
    }

    /* Compute missing terms in order */
    if (computed_len <= n) {
        for (idx_t i = computed_len; i <= n; ++i) {
            if (i == 0) {
#ifndef __cplusplus
                cache[i] = (seq_t)1;
#else
                cache[i] = seq_t(1);
#endif
            } else if (i == 1) {
#ifndef __cplusplus
                cache[i] = (seq_t)1;
#else
                cache[i] = seq_t(1);
#endif
            } else if (i == 2) {
#ifndef __cplusplus
                cache[i] = (seq_t)2;
#else
                cache[i] = seq_t(2);
#endif
            } else if (i == 3) {
#ifndef __cplusplus
                cache[i] = (seq_t)5;
#else
                cache[i] = seq_t(5);
#endif
            } else if (i == 4) {
#ifndef __cplusplus
                cache[i] = (seq_t)14;
#else
                cache[i] = seq_t(14);
#endif
            } else {
#ifdef __cplusplus
                seq_t t1 = seq_t(-10*(int64_t)i + 12) * cache[i-1];
                seq_t t2 = seq_t(34*(int64_t)i - 78) * cache[i-2];
                seq_t t3 = seq_t(-48*(int64_t)i + 162) * cache[i-3];
                seq_t t4 = seq_t(25*(int64_t)i - 114) * cache[i-4];
                seq_t t5 = seq_t(-2*(int64_t)i + 12) * cache[i-5];
                seq_t sum = t1 + t2 + t3 + t4 + t5;
                cache[i] = -sum / (seq_t)i;
#else
                __int128_t t1 = ((__int128_t)(-10*(int64_t)i + 12)) * cache[i-1];
                __int128_t t2 = ((__int128_t)(34*(int64_t)i - 78)) * cache[i-2];
                __int128_t t3 = ((__int128_t)(-48*(int64_t)i + 162)) * cache[i-3];
                __int128_t t4 = ((__int128_t)(25*(int64_t)i - 114)) * cache[i-4];
                __int128_t t5 = ((__int128_t)(-2*(int64_t)i + 12)) * cache[i-5];
                __int128_t sum = t1 + t2 + t3 + t4 + t5;
                cache[i] = (seq_t)(-sum / ( __int128_t)i);
#endif
            }
        }
        computed_len = n + 1;
    }

    return cache[n];
}
