/*
 * A365406  Numbers j whose largest divisor <= sqrt(j) is a power of 2.
 * https://oeis.org/A365406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer j such that the largest divisor ≤ sqrt(j) is
 *            a power of 2
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9024 (2026-11-01)
 * Helpers:   is_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_pow2.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

static inline bool j_qualifies(idx_t j)
{
    idx_t lim = isqrt_u64(j);
    for (idx_t d = lim; d > 0; --d) {
        if (j % d == 0)
            return is_pow2((seq_t)d);
    }
    return false;   /* unreachable – 1 always divides */
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 1;

    while (cache_len < n) {
        idx_t j = next_candidate;

        if (j_qualifies(j)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                if (new_cap < cache_len + 1)
                    new_cap = cache_len + 1;
                if (cache == NULL)
                    cache = seq_alloc(new_cap);
                else
                    cache = seq_realloc(cache, cache_cap, new_cap);
                if (!cache)
                    return 0;   /* allocation failure – unlikely */
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)j;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
