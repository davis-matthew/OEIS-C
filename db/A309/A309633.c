/*
 * A309633  G.f.: x * Sum_{k>=1} x^k / (1 - a(k)*x^k).
 * https://oeis.org/A309633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n+1) = Σ_{d|n} a(d)^(n/d−1)
 * Range:     u8 n <= 42, u16 n <= 84, u32 n <= 154, u64 n <= 294,
 *            u128 n <= 574, big any n
 * Validated: standard, pass, 96 terms through n = 9467 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
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
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* how many elements are allocated (index 0 unused) */
    static size_t last  = 0;   /* largest index that has been computed */

    if (n < 1)            /* offset is 1 */
        return 0;

    /* allocate or grow cache so that indices [0 … n] are available */
    if (cap < (size_t)n + 1) {
        ensure_cache((idx_t)cap, &cache, &cap);   /* ensure capacity up to n+1 */
        /* after ensure_cache we are guaranteed cap >= n+1   */
        if (cap < (size_t)n + 1)  /* safety: may reallocate again */
            ensure_cache((idx_t)n, &cache, &cap);
    }

    /* compute all missing values up to n */
    if (last < (size_t)n) {
        for (idx_t k = (idx_t)last + 1; k <= n; ++k) {
            if (k == 1) {
                cache[1] = 0;
            } else {
                idx_t m   = k - 1;
                seq_t sum = 0;

                /* scan all divisor pairs d, m/d */
                for (idx_t d = 1; d <= m / d; ++d) {
                    if (m % d == 0) {
                        /* divisor d */
                        idx_t exp1 = (m / d) - 1;
                        sum += ipow_idx(cache[d], exp1);

                        /* divisor m/d (different from d) */
                        idx_t d2 = m / d;
                        if (d2 != d) {
                            idx_t exp2 = d - 1;
                            sum += ipow_idx(cache[d2], exp2);
                        }
                    }
                }
                cache[k] = sum;
            }
        }
        last = (size_t)n;
    }

    return cache[n];
}
