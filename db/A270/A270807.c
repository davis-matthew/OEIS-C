/*
 * A270807  Trajectory of 1 under the map n -> n + n/gpf(n) + 1 (see A269304).
 * https://oeis.org/A270807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = a(n) + a(n)/gpf(a(n)) + 1
 * Range:     u8 n <= 44, u16 n <= 660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* largest computed index */

    if (n <= cache_len && cache != NULL)
        return cache[n];

    /* Initial allocation */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 1;
    }

    /* Grow the table if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];
        uint64_t pu  = (uint64_t)prev;   /* safe: values fit in 64‑bit */
        uint64_t pf  = gpf(pu);
        seq_t inc    = prev / (seq_t)pf;
        cache[i]     = prev + inc + 1;
    }

    cache_len = n;
    return cache[n];
}
