/*
 * A329928  a(n) = (Pi/2)*(2*n+1)!*binomial(2*n+1, (2*n+1)/2).
 * https://oeis.org/A329928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*(4*n)^2; a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  hi    = 0;

    if (cache == NULL) {
        ensure_cache((idx_t)0, &cache, &cap);
        cache[0] = (seq_t)2;
        hi = 0;
    }

    if (n > hi) {
        ensure_cache(n, &cache, &cap);
        for (idx_t i = hi + 1; i <= n; ++i) {
            /* multiplier = (4*i)^2 = 16*i*i */
            seq_t mult = (seq_t)(16ULL * (unsigned long long)i * (unsigned long long)i);
            cache[i] = seq_mul(cache[i - 1], mult);
        }
        hi = n;
    }

    return cache[n];
}
