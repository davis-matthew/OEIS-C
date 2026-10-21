/*
 * A287292  a(1) = 1, a(2) = 2; a(n) = a(n-a(n-1)+1) + a(n-a(n-2)-1) for n >
 *          2.
 * https://oeis.org/A287292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-a(n-1)+1) + a(n-a(n-2)-1); a(1)=1, a(2)=2
 * Range:     u8 n <= 491, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>
#include <string.h>
#endif

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.size() <= (size_t)n) {
        cache.resize(n + 1);
    }
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }
    if (n == 2) {
        cache[2] = 2;
        return 2;
    }
    if (cache[n] != 0) {
        return cache[n];
    }
    seq_t a_nm1 = a(n - 1);
    seq_t a_nm2 = a(n - 2);
    idx_t idx1 = n - (idx_t)a_nm1 + 1;
    idx_t idx2 = n - (idx_t)a_nm2 - 1;
    seq_t val1 = a(idx1);
    seq_t val2 = a(idx2);
    cache[n] = val1 + val2;
    return cache[n];
#else
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    idx_t required = n + 1;
    if (cap < required) {
        idx_t new_cap = (cap == 0 ? 4 : cap);
        while (new_cap < required) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        for (idx_t i = cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cap = new_cap;
    }
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }
    if (n == 2) {
        cache[2] = 2;
        return 2;
    }
    if (cache[n] != 0) {
        return cache[n];
    }
    seq_t a_nm1 = a(n - 1);
    seq_t a_nm2 = a(n - 2);
    idx_t idx1 = n - (idx_t)a_nm1 + 1;
    idx_t idx2 = n - (idx_t)a_nm2 - 1;
    seq_t val1 = a(idx1);
    seq_t val2 = a(idx2);
    cache[n] = val1 + val2;
    return cache[n];
#endif
}
