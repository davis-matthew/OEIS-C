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
 * Helpers:   is_power_of_two, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_power_of_two.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

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
            return is_power_of_two((seq_t)d);
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
