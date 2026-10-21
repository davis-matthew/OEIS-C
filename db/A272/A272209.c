/*
 * A272209  Number of partitions of the number of divisors of n.
 * https://oeis.org/A272209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(d(n)) – the partition number of the divisor count of n
 * Range:     u8 n <= 179, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, seq_realloc_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_realloc_safe.h"

seq_t a(idx_t n)
{
    /* 1. compute the divisor count d(n) */
    uint64_t d64 = divisor_count_u64((uint64_t)n);
    idx_t d = (idx_t)d64;                /* fits in idx_t */

    /* 2. static cache for partition values p(0 .. d)   */
    static seq_t *p_cache = NULL;
    static size_t cache_cap = 0;         /* allocated capacity */
    static size_t cache_size = 0;        /* number of computed elements */

    /* 3. initialise cache if first call */
    if (!p_cache) {
        cache_cap = 1;                   /* need element 0 */
        p_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!p_cache) abort();
        p_cache[0] = 1;                  /* p(0) = 1  */
        cache_size = 1;
    }

    /* 4. grow cache if necessary so that index d is reachable. */
    if (cache_cap <= (size_t)d) {
        size_t new_cap = d + 1;          /* at least d+1 elements */
        if (new_cap <= cache_cap) new_cap = cache_cap * 2;
        p_cache = seq_realloc_safe(p_cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* 5. compute missing partition values up to d. */
    for (idx_t k = cache_size; k <= d; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t j = 1; ; ++j) {
            idx_t g1 = j * (3 * j - 1) / 2;
            if (g1 > k) break;
            seq_t add = p_cache[k - g1];
            if (j & 1) pos += add; else neg += add;

            idx_t g2 = j * (3 * j + 1) / 2;
            if (g2 > k) continue;
            add = p_cache[k - g2];
            if (j & 1) pos += add; else neg += add;
        }
        p_cache[k] = pos - neg;          /* pos ≥ neg for all k */
    }
    cache_size = d + 1;                 /* up to d now computed */

    /* 6. return the requested value */
    return p_cache[d];
}
