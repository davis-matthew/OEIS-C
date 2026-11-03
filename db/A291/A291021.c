/*
 * A291021  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S - S^2 - S^3 + S^4
 *          + S^5.
 * https://oeis.org/A291021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 14*a(n-3) - 9*a(n-4) + 2*a(n-5)
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=25, a(4)=67
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 956 (2026-11-03)
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

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* compute the next value using the recurrence */
static inline seq_t compute_next(idx_t i, seq_t *buf)
{
    /* seq_t arithmetic will work for both builtin integer types
       and arbitrary‑precision classes. */
    seq_t res = 6 * buf[i-1];
    res -= (seq_t)13 * buf[i-2];
    res += (seq_t)14 * buf[i-3];
    res -= (seq_t)9 * buf[i-4];
    res += (seq_t)2 * buf[i-5];
    return res;
}

/* public function */
seq_t a(idx_t n)
{
    /* initialise cache with the first five terms */
    if (cache_len == 0) {
        ensure_cache(5, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)67;
        cache_len = 5;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence applies for i >= 5, for i < 5 values are already set */
        if (i < 5) continue;
        cache[i] = compute_next(i, cache);
    }
    cache_len = n + 1;

    return cache[n];
}
