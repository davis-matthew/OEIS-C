/*
 * A332145  a(n) = 4*(10^(2*n+1)-1)/9 + 10^n.
 * https://oeis.org/A332145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Recurrence: a(0)=5, a(1)=454, a(2)=44544; *
 *            a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) for n>2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms already stored   */
    static size_t cache_cap = 0;  /* allocated capacity in terms       */

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow the cache so it can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);   /* guarantees n+1 slots */

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 5;
        } else if (i == 1) {
            val = 454;
        } else if (i == 2) {
            val = 44544;
        } else {
            /* recurrence: a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) */
            val = 111 * cache[i - 1] - 1110 * cache[i - 2] + 1000 * cache[i - 3];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
